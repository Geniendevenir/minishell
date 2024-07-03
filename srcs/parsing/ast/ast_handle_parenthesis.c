/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_handle_parenthesis.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 19:49:19 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/01 19:54:39 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_parenthesis_open(t_token **tokens, t_ast **current, t_ast **root)
{
	if ((*tokens)->type == TOKEN_OPENPAR)
	{
		*current = open_parenthesis(tokens, *current);
		if (!*root)
			*root = *current;
	}
}

t_ast	*open_parenthesis(t_token **tokens, t_ast	*current)
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

t_ast	*close_parenthesis(t_token **tokens, t_ast	*root)
{
	t_token	*temp;

	free_token_and_next_in_ast(tokens, &temp);
	return (root);
}