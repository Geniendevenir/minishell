/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 17:32:39 by Matprod           #+#    #+#             */
/*   Updated: 2024/08/13 17:25:03 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_is_in_env(t_env *env, char *unset)
{
	t_env *current;

	current = env;
	while (current)
	{
		if (ft_strcmp(current->key, unset) != 0)
			return (1);
		current = current->next;
	}
	return (0);
}

void	ft_destroy(t_env *env, char *unset)
{
	t_env			*tmp;

	while (env && env->next)
	{
		if (ft_strcmp(env->next->key, "PWD") == 0
			&& ft_strcmp(env->next->key, unset) == 0)
		{
			env->next->code = 4;
			return ;
		}
		if (!ft_strncmp(env->next->key, unset, ft_strlen(unset)))
		{
			tmp = env->next;
			env->next = tmp->next;
			if (tmp->key && tmp->key != NULL)
				free(tmp->key);
			if (tmp->value && tmp->value != NULL)
				free(tmp->value);
			free(tmp);
		}
		env = env->next;
	}
}

int	ft_unset(t_env *env, char **unset)
{
	unsigned int	i;

	i = 0;
	while (unset[++i])
	{
		if (ft_is_in_env(env, unset[i]))
			ft_destroy(env, unset[i]);
	}
	return (EXIT_SUCCESS);
}
