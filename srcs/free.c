/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:34:27 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/30 19:00:23 by allan            ###   ########.fr       */
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

void free_ast(t_ast *node) 
{
	if (node == NULL)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	if (node->value)
		free(node->value);
	free(node);
}void free_ast(t_ast *node) 
{
	if (node == NULL)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	if (node->value)
		free(node->value);
	free(node);
}