/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:34:27 by Matprod           #+#    #+#             */
/*   Updated: 2024/09/15 13:47:27 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_env(t_env *envp)
{
	t_env	*temp;

	if (!envp || envp == NULL)
		return ;
	while (envp != NULL)
	{
		if (!(!envp->key || envp->key == NULL))
			free(envp->key);
		if (!(!envp->value || envp->value == NULL))
			free(envp->value);
		temp = envp;
		envp = envp->next;
		if (temp)
			free(temp);
	}
	return ;
}

void	free_all(t_all *p)
{
	close(p->std_in);
	close(p->std_out);
	if (p->line)
		free(p->line);
	if (p->env)
		free_env(p->env);
	if (p->sig)
		free(p->sig);
	if (p)
		free(p);
	rl_clear_history();
}

void	free_array(char **array)
{
	int	i;

	i = -1;
	while (array[++i])
	{
		if (array[i])
			free(array[i]);
	}
	if (array)
		free(array);
}

