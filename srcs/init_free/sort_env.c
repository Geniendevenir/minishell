/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 19:01:28 by Matprod           #+#    #+#             */
/*   Updated: 2024/08/27 15:04:50 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static unsigned int	size_env(char **env)
{
	unsigned int	len;

	len = -1;
	while (env[++len])
		;
	return (len);
}

static char	*get_join(t_env *env)
{
	char	*str;

	if (env->code == 1)
	{
		str = ft_strjoin(env->key, "=\"");
		str = ft_strjoin_spe(str, env->value);
		str = ft_strjoin_spe(str, "\"");
	}
	else if (env->code == 2)
		str = ft_strjoin(env->key, "=\"\"");
	else
		str = ft_strdup(env->key);
	return (str);
}

char	**env_to_char_export(t_env *env)
{
	char	**reforged;
	int		i;
	int		e;
	t_env	*temp;

	i = 0;
	e = 0;
	if (!env || env == NULL)
		return (NULL);
	temp = env;
	while (temp != NULL && i++ > -1)
		temp = temp->next;
	reforged = malloc((i + 1) * sizeof(char *));
	if (!reforged)
		return (NULL);
	while (e < i)
	{
		reforged[e] = get_join(env);
		if (reforged[e++] == NULL)
			return (free_array(reforged), NULL);
		env = env->next;
	}
	return (reforged[e] = NULL, reforged);
}

char	**sort_env(char **env)
{
	unsigned const int	size = size_env(env);
	unsigned int		i;
	unsigned int		j;
	char				*tmp;

	i = -1;
	while (++i < size - 1)
	{
		j = i;
		while (++j < size)
		{
			if (ft_strcmp(env[i], env[j]) > 0)
			{
				tmp = env[i];
				env[i] = env[j];
				env[j] = tmp;
			}
		}
	}
	i = -1;
	while (env[++i] != NULL)
		printf("declare -x %s\n", env[i]);
	return (free_array(env), NULL);
}
