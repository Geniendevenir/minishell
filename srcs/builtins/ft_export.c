/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 12:53:21 by allan             #+#    #+#             */
/*   Updated: 2024/06/27 21:32:31 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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

int	ft_export(char **new_env, t_env **env_list)
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
}
