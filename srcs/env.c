/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:25:36 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/01 15:36:41 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_env	*init_env(t_env *p_env, char **env)
{
	p_env = malloc(sizeof(t_env));
	if (!p_env)
		return (NULL);
	p_env = env_to_struct(env);
	if (!p_env)
		return (free(p_env), NULL);
	if (create_signal() == -1)
		return (free(p_env), NULL);
	g_sig.cmd_stat = 0;
	return (p_env);
}

t_env	*ft_envlast(t_env *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_env_add_back(t_env **lst, t_env *new)
{
	t_env	*tmp;

	if (*lst)
	{
		tmp = ft_envlast(*lst);
		tmp->next = new;
	}
	else
		*lst = new;
}

t_env	*ft_envnew(char *key, char *value)
{
	t_env	*list;

	list = malloc(sizeof(t_env));
	if (!list)
		return (NULL);
	list->key = key;
	if (ft_strcmp(list->key, "SHLVL") == 0)
	{
		list->value = ft_itoa(ft_atoi(value) + 1);
		free(value);
	}
	else
		list->value = value;
	list->next = NULL;
	list->code = 1;
	return (list);
}

t_env	*env_to_struct(char **env)
{
	char			*tmp;
	char			*value;
	t_env			*tmp_env;
	t_env			*env_struct;
	unsigned int	i;

	env_struct = NULL;
	while (*env)
	{
		i = -1;
		while ((*env)[++i] && (*env)[i] != '=')
			;
		tmp = ft_strndup(*env, i);
		if (!tmp)
			return (NULL);
		value = ft_strdup(*env + i + 1);
		if (!value)
			return (free(tmp), NULL);
		tmp_env = ft_envnew(tmp, value);
		if (!tmp_env)
			return (free(tmp), free(value), NULL);
		ft_env_add_back(&env_struct, tmp_env);
		env++;
	}
	return (env_struct);
}