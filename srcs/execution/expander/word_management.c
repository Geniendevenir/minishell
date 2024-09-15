/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_management.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 23:02:14 by allan             #+#    #+#             */
/*   Updated: 2024/09/15 16:34:04 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*replace_word(t_ast **root, t_ast *node, t_ast *new_node)
{
	if (!new_node)
	{
		new_node = create_node(NULL, 0);
		if (!new_node)
			return (NULL);
	}
	if (node->type == WORD_CMD)
	{
		if (node->left)
			node->left->type = WORD_CMD;
	}
	if (new_node->value != NULL)
		new_node->parent = node->parent;
	if (node->parent == NULL)
		return (replace_word_next(root, node, new_node, 1));
	else if (node->parent->left == node)
		return (replace_word_next(root, node, new_node, 2));
	else if (node->parent->right == node)
		return (replace_word_next(root, node, new_node, 3));
	return (node);
}

t_ast	*replace_word_next(t_ast **root, t_ast *node, t_ast *new_node
, int option)
{
	if (option == 1)
	{
		*root = new_node;
		return (new_node);
	}
	else if (option == 2)
	{
		node->parent->left = new_node;
		new_node->parent = node->parent;
		return (new_node);
	}
	else if (option == 3)
	{
		node->parent->right = new_node;
		new_node->parent = node->parent;
		return (new_node);
	}
	return (new_node);
}

void	delete_word(t_ast **root, t_ast **node)
{
	t_ast	*temp;

	temp = *node;
	if ((*node) == NULL)
		return ;
	if ((*node)->left == NULL)
	{
		*node = replace_word(root, *node, NULL);
		if ((*node)->parent)
			*node = (*node)->parent;
	}
	else
		*node = replace_word(root, *node, (*node)->left);
	free(temp->value);
	free(temp);
}

bool	insert_word(t_ast **node, t_token *token)
{
	t_ast	*new_node;

	new_node = create_node(token, (*node)->subshell);
	if (!new_node)
		return (1);
	if ((*node)->type == WORD_CMD)
		new_node->type = WORD_OPTION;
	else
		new_node->type = WORD_CMD;
	new_node->state = (*node)->state;
	if ((*node)->left)
	{
		new_node->left = (*node)->left;
		(*node)->left->parent = new_node;
	}
	(*node)->left = new_node;
	new_node->parent = *node;
	return (0);
}

bool	modify_word(t_ast **node, t_token *token_list)
{
	char	*temp;

	temp = (*node)->value;
	(*node)->value = ft_strdup(token_list->value);
	free(temp);
	if (!(*node)->value)
		return (1);
	return (0);
}
