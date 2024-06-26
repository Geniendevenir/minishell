/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_handle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:48:03 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/26 15:57:24 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*handleOption(t_token **tokens, t_ast* current)
{
	t_ast*	newNode;
	t_ast*	temp;
	
	newNode = createNode((*tokens)->type, (*tokens)->value);
	if (current && current->type == WORD_BUILTIN)
	{
		if (current->left)
		{
			temp = current->left;
			while (temp->left)
				temp = temp->left;
			temp->left = newNode;
			newNode->parent = temp;
		}
		else
		{
			current->left = newNode;
			newNode->parent = current;
		}
	}
	else
	{
		if (current)
		{
			swap_child_left(current, newNode);
		}
		else
			current = newNode;
	}
	*tokens = (*tokens)->next;
	return current;
}

t_ast	*handleOpenParenthesis(t_token **tokens, t_ast* current)
{
	*tokens = (*tokens)->next;
	t_ast* subTree = parseSubexpression(tokens);
	if (current)
	{
		current->right = subTree;
		subTree->parent = current;
		current = subTree;
	}
	else
		current = subTree;
	return current;
}

t_ast	*handleCloseParenthesis(t_token **tokens, t_ast* root)
{
	*tokens = (*tokens)->next;
	return (root);
}

t_ast	*handle_priorities(t_token **tokens, t_ast* root)
{
	t_ast	*newNode;

	newNode = createNode((*tokens)->type, (*tokens)->value);
    newNode->left = root;
    if (root)
		root->parent = newNode;
    root = newNode;
	*tokens = (*tokens)->next;
    return root;
}

t_ast	*handleBuiltinCmdQuotes(t_token **tokens, t_ast* current)
{
	t_ast* newNode;
	
	newNode = createNode((*tokens)->type, (*tokens)->value);
	if (current)
	{
		current->right = newNode;
		newNode->parent = current;
		current = newNode;
	}
	else
		current = newNode;
	*tokens = (*tokens)->next;
	return current;
}