/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_handle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:48:03 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/26 22:07:30 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*handle_open_parenthesis(t_token **tokens, t_ast	*current)
{
	t_ast	*sub_tree;
	t_token	*temp;

	free_token_and_next_in_ast(tokens, &temp);
	sub_tree = parse_subexpression(tokens);
	if (current)
	{
		current->right = sub_tree;
		sub_tree->parent = current;
		current = sub_tree;
	}
	else
		current = sub_tree;
	return (current);
}

t_ast	*handle_close_parenthesis(t_token **tokens, t_ast	*root)
{
	t_token	*temp;

	free_token_and_next_in_ast(tokens, &temp);
	return (root);
}

t_ast	*handle_priorities(t_token **tokens, t_ast	*root)
{
	t_ast	*new_node;
	t_token	*temp;

	new_node = create_node((*tokens)->type, (*tokens)->value);
	if (!new_node)
		return (NULL);
	new_node->left = root;
	if (root)
		root->parent = new_node;
	root = new_node;
	free_token_and_next_in_ast(tokens, &temp);
	return (root);
}

t_ast	*handle_builtin_and_cmd(t_token **tokens, t_ast	*current)
{
	t_ast	*new_node;
	t_token	*temp;

	new_node = create_node((*tokens)->type, (*tokens)->value);
	if (current)
	{
		current->right = new_node;
		new_node->parent = current;
		current = new_node;
	}
	else
		current = new_node;
	free_token_and_next_in_ast(tokens, &temp);
	return (current);
}

t_ast	*handle_option(t_token **tokens, t_ast *current)
{
	t_ast	*new_node;
	t_ast	*temp;
	t_token	*tmp;

	new_node = create_node((*tokens)->type, (*tokens)->value);
	if (current && current->type == WORD_BUILTIN)
		part_handle_option(&current, &new_node, &temp);
	else
	{
		if (current)
			swap_child_left(current, new_node);
		else
			current = new_node;
	}
	free_token_and_next_in_ast(tokens, &tmp);
	return (current);
}
