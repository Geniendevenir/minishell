/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:34:27 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/01 15:35:12 by Matprod          ###   ########.fr       */
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