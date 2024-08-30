/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 12:53:21 by allan             #+#    #+#             */
/*   Updated: 2024/08/27 18:25:39 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	change_value_empty(t_env *env, char *key)
{
	t_env	*new;

	if (!ft_is_in_env(env, key))
	{
		new = ft_envnew(key, NULL);
		if (!new)
			return (free(key));
		new->code = 3;
		ft_env_add_back(&env, new);
	}
}

int	before(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (-1);
	if (!ft_isalpha(str[1]))
		return (ft_putstr_fd("bash: export: '", 2), ft_putstr_fd(str, 2),
			ft_putendl_fd("': not a valid identifier", 2), -1);
	while (str[i])
	{
		if (str[i] == '+' && str[i + 1] == '=')
			break ;
		else if (ft_isalpha(str[i]) == 0 && str[i] != '=')
			return (ft_putstr_fd("bash: export: '", 2), ft_putstr_fd(str, 2),
				ft_putendl_fd(": not a valid identifier", 2), -1);
		else if (str[i] == '=')
			break ;
		++i;
	}
	return (i);
}

int	change_concat(t_env *env, char *key, char *cmd)
{
	char	*value;
	char	*key_not_exist;

	value = NULL;
	if (ft_is_in_env(env, key))
	{
		if (get_env_var(env, key))
			value = ft_strjoin(get_env_var(env, key), cmd + ft_strlen(key) + 2);
		if (!value)
			return (free(key), EXIT_FAILURE);
		change_value(env, key, value);
	}
	else
	{
		key_not_exist = ft_strdup(key);
		if (!key_not_exist)
			return (free(key), EXIT_FAILURE);
		value = ft_strdup(cmd + ft_strlen(key) + 2);
		if (!value)
			return (free(key), free(key_not_exist), EXIT_FAILURE);
		change_value(env, key_not_exist, value);
		free(key);
	}
	return (EXIT_SUCCESS);
}

int	do_export(t_env *env, char *key, char *cmd)
{
	char	*value;

	if (cmd[ft_strlen(key)] == '+'
		&& cmd[ft_strlen(key) + 1] == '=')
		return (change_concat(env, key, cmd));
	value = ft_strdup(cmd + ft_strlen(key) + 1);
	if (!value)
		return (free(key), 1);
	change_value(env, key, value);
	return (0);
}

int	ft_export(t_env *env, char **cmd)
{
	char	*key;
	int		i;

	printf("CMD[1] = '%s'\n",cmd[1]);
	if (!cmd[1])
		return (sort_env(env_to_char_export(env)), 0);
	i = 0;
	while (cmd[++i])
	{
		if (before(cmd[i]) == -1)
			return (1);
		key = ft_strndup(cmd[i], before(cmd[i]));
		if (!key)
			return (1);
		if (cmd[i][ft_strlen(key)] == '=' && !(cmd[i][ft_strlen(key) + 1]))
			change_value_equal(env, key);
		else if (!cmd[i][ft_strlen(key)])
			change_value_empty(env, key);
		else
			do_export(env, key, cmd[i]);
	}
	return (0);
}

/*
typedef struct s_env
{
	char			*key;
	char			*value;
	int				code;
	struct s_env	*next;
}	t_env;
*/

/*
	CASE 1: Set an ENV with export KEY=VALUE
	
	check si plusieurs env: export ENV1 ENV2 ENV3
	check syntax: KEY respecte ENV syntax et VALUE a une KEY such as KEY=VALUE (=VALUE is incorrect);
	check format: KEY / KEY=VALUE
	check si existe deja: 
		-> Si oui et FORMAT KEY et a deja une valeur: Remplacer (TEST= est considere comme une valeur)
		-> Si oui et FORMAT KEY mais n'a pas de valeur: (Ne pas remplacer)
		-> Si non si a une valeur rajouter
*/

//printf("test1\n");

/* int	ft_export(char **new_env, t_env **env_list) //CHECK SYNTAX POUR VALUE (ex test=& > Error)
{
	int		i;
	int		len;

	i = 0;
	if (!new_env)
		return (1);
	while (new_env[i])
	{
		len = ft_strlen(new_env[i]);
		if (valid_export(new_env[i]) == 0)
		{
			if (split_env(new_env[i], len, env_list) == 1)
				return (1);
		}
		else
		{
			write(2, "bash: export: `", 15);
			write(2, new_env[i], ft_strlen(new_env[i]));
			write(2, "': not a valid identifier\n", 26);
		}
		i++;
	}
	return (0);
}


int split_env(char *new_env, int len, t_env **env_list)
{
	t_env *add_env;
	int		i;

	i = ft_strichr(new_env, '=');
	if (i == -1) //check si cas particulier ou une variable existe deja et on l'export
		return (0);
	add_env = env_init();
	if (!add_env) //*error
		return (1); //malloc
	add_env->key = ft_substr(new_env, 0, i);
	if (!add_env->key)
		return (export_free(&add_env, 0));
	if ((len - i) == 0)
	{
		add_env->value = malloc(sizeof(char) * 1);
		if (!add_env->value)
			return (export_free(&add_env, 1));
		add_env->value[0] = '\0';
	}
	else
		add_env->value = ft_substr(new_env, i + 1, len - i);
	if (!add_env->value) // && (len - i != 0)
		return (export_free(&add_env, 1));
	ft_env_add_back(env_list, add_env);
	return (0);
}

int	valid_export(char *new_env)
{
	int	i;

	i = 0;
	if (!(new_env[i] >= 'A' && new_env[i] <= 'Z') && !(new_env[i] >= 'a' && new_env[i] <= 'z') && new_env[i] != '_')
		return (1);
	i++;
	if (new_env[i] == '=')
		return (0);
	while (new_env[i] && new_env[i] != '=')
	{
		
		if ((new_env[i] >= 'A' && new_env[i] <= 'Z') || (new_env[i] >= 'a' && new_env[i] <= 'z') || (new_env[i] == '_') 
		|| (new_env[i] >= '0' && new_env[i] <= '9'))
			i++;
		else
			return (1);
	}
	return (0);
}

t_env	*env_init(void)
{
	t_env	*add_env;

	add_env = malloc(sizeof(t_env));
	if (!add_env)
		return (NULL);
	add_env->key = NULL;
	add_env->value = NULL;
	add_env->code = 0;
	add_env->next = NULL;
	return (add_env);
}

int	export_free(t_env **add_env, int option)
{
	if (option == 0)
		free(*add_env);
	else if (option == 1)
	{
		free((*add_env)->key);
		free(*add_env);
	}
	return (1);
} */
