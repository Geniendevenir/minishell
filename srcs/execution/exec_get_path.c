/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_get_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 13:26:25 by allan             #+#    #+#             */
/*   Updated: 2024/07/11 17:26:25 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_path(t_env *env)
{
	while (env)
	{
		if (ft_strcmp(env->key, "PATH") == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

char	*path_free(t_path *p, int *error, int option)
{
	if (option == 1)
		free_array(p->env_paths);
	else if (option == 2)
	{
		free_array(p->env_paths);
		return (p->final_path);
	}
	else if (option == 3)
	{
		free_array(p->env_paths);
		free(p->final_path);
		return (NULL);
	}
	*error = 1;
	return (NULL);
}

void	p_init(t_path *p)
{
	p->path_value = NULL;
	p->env_paths = NULL;
	p->final_path = NULL;
	p->part_path = NULL;
	p->i = 0;
}

char	*get_path(const char *cmd, t_env *env, int *error)
{
	t_path 	p;

	p_init(&p);
	if (!env) // If no ENV
		return (NULL);
	p.path_value = find_path(env);
	if (!p.path_value || !(*p.path_value))
		return (NULL);
	p.env_paths = ft_split(p.path_value, ':');
	if (!p.env_paths)
		return (path_free(&p, error, 4));
	//print_tab(p.env_paths);
	while (p.env_paths[p.i])
	{
		p.part_path = ft_strjoin(p.env_paths[p.i], "/");
		if (!p.part_path)
			return (path_free(&p, error, 1));
		p.final_path = ft_strjoin(p.part_path, cmd);
		free(p.part_path);
		if (!p.final_path)
			return (path_free(&p, error, 1));
		if (access(p.final_path, X_OK) == 0)
			return (path_free(&p, error, 2));
		p.i++;
	}
	path_free(&p, error, 3);
	return (NULL);
}
