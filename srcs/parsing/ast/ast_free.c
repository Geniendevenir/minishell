/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 13:26:01 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/06 13:29:34 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_ast(t_ast *node)
{
	if (node == NULL)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	if (node->value)
		free(node->value);
	free(node);
}

void	free_list_ptr(t_ast_ptr **list, t_ast **temp_free, int option)
{
	if (option == 1)
	{
		*temp_free = (*list)->root;
		free((*list));
	}
	else if (option == 2)
	{
		*temp_free = (*list)->current;
		free((*list));
	}
}
