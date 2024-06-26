/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:50:17 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/26 19:56:06 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*create_node(enum s_type type, char	*value)
{
	t_ast	*node;

	node = (t_ast *)malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->value = ft_strdup(value);
	if (!node->value)
		return (NULL);
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;
	return (node);
}

void	swap_child_left(t_ast	*current, t_ast	*newNode)
{
	if (current == NULL || newNode == NULL)
		return ;
	current->left = newNode;
	newNode->parent = current;
	current = newNode;
}

void	swap_child_right(t_ast	*current, t_ast	*newNode)
{
	if (current == NULL || newNode == NULL)
		return ;
	current->left = newNode;
	newNode->parent = current;
	current = newNode;
}

void	part_handle_option(t_ast **current, t_ast **new_node, t_ast **temp)
{
	if ((*current)->left)
	{
		(*temp) = (*current)->left;
		while ((*temp)->left)
			(*temp) = (*temp)->left;
		(*temp)->left = (*new_node);
		(*new_node)->parent = *temp;
	}
	else
	{
		(*current)->left = (*new_node);
		(*new_node)->parent = (*current);
	}
}
