/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_management.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 23:02:14 by allan             #+#    #+#             */
/*   Updated: 2024/07/11 18:48:34 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*replace_word(t_ast **root, t_ast *node, t_ast *new_node)
{
	if (node->type == WORD_CMD)
	{
		if (node->left)
			node->left->type = WORD_CMD;
	}
	if (new_node != NULL)
		new_node->parent = node->parent;
	if (node->parent == NULL)
	{
		*root = new_node;
		return (new_node);
	}
	else if (node->parent->left == node)
	{
		node->parent->left = new_node;
		return (new_node);
	}
	return (node);
}

/*
L49	// Case 2: (*Node) is a leaf
L51	// Case 3: (*Node) has only left child
*/
void	delete_word(t_ast **root, t_ast **node)
{
	t_ast	*temp;

	temp = *node;
	if ((*node) == NULL)
		return ;
	if ((*node)->left == NULL)
		*node = replace_word(root, *node, NULL);
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

int	handle_wildcard(t_ast **current, t_token **token_list)
{
	t_token	*token;
	int		error;

	token = *token_list;
	error = 0;
	if (is_file(*current))
		error_expander(*current, 1);
	else if ((*current)->type == WORD_CMD || (*current)->type == WORD_OPTION)
	{
		if (ft_strcmp((*current)->value, token->value) != 0)
		{
			error = modify_word(current, *token_list);
			if (error != 0)
				return (error);
		}
		token = token->next;
		while (token)
		{
			if (insert_word(current, token) == 1)
				return (1);
			*current = (*current)->left;
			token = token->next;
		}
	}
	return (0);
}
