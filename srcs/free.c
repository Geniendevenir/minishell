/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:34:27 by Matprod           #+#    #+#             */
/*   Updated: 2024/09/11 02:18:54 by allan            ###   ########.fr       */
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
		free(temp);
	}
	return ;
}

void	free_all(t_all *p)
{
	close(p->std_in);
	close(p->std_out);
	free(p->line);
	free_env(p->env);
	free(p->sig);
	free(p);
}

void	free_array(char **array)
{
	int	i;

	i = -1;
	while (array[++i])
		free(array[i]);
	free(array);
}

