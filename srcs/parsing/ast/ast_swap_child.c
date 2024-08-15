/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_swap_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:50:17 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/05 17:36:01 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	swap_child_left(t_ast	*current, t_ast	*new_node)
{
	if (current == NULL || new_node == NULL)
		return ;
	current->left = new_node;
	new_node->parent = current;
	current = new_node;
}

void	swap_child_right(t_ast	*current, t_ast	*new_node)
{
	if (current == NULL || new_node == NULL)
		return ;
	current->right = new_node;
	new_node->parent = current;
	current = new_node;
}

void	swap_child_left_with_else(t_ast	*current, t_ast	*new_node)
{
	if (current != NULL && new_node != NULL)
	{
		current->left = new_node;
		new_node->parent = current;
		current = new_node;
	}
	else
		current = new_node;
}

void	swap_child_right_with_else(t_ast	*current, t_ast	*new_node)
{
	if (current != NULL || new_node != NULL)
	{
		current->right = new_node;
		new_node->parent = current;
		current = new_node;
	}
	else
		current = new_node;
}
