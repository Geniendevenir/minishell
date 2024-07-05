/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_swap_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:50:17 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/05 16:56:35 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*create_node(enum s_type type, char	*value, int subshell)
{
	t_ast	*node;

	node = (t_ast *)malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->value = ft_strdup(value);
	if (!node->value)
		return (NULL);
	node->subshell = subshell;
	node->exit_state = 0;
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;
	return (node);
}

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
