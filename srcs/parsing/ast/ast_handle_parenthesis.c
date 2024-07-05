/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_handle_parenthesis.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 19:49:19 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/05 17:11:25 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_parenthesis_open(t_token **tokens, t_ast_ptr **list,
	int sub_shell)
{
	if ((*tokens)->type == TOKEN_OPENPAR)
	{
		(*list)->current = open_parenthesis(tokens, (*list)->current,
				sub_shell);
		if (!(*list)->root)
			(*list)->root = (*list)->current;
	}
}

t_ast	*open_parenthesis(t_token **tokens, t_ast	*current, int sub_shell)
{
	t_ast	*sub_tree;
	t_token	*temp;

	free_token_and_next_in_ast(tokens, &temp);
	sub_tree = parse_subexpression(tokens, sub_shell);
	if (current)
	{
		current->right = sub_tree;
		sub_tree->parent = current;
		current = sub_tree;
	}
	else
	{
		current = sub_tree;
	}
	return (current);
}

t_ast	*close_parenthesis(t_token **tokens, t_ast	*root)
{
	t_token	*temp;

	free_token_and_next_in_ast(tokens, &temp);
	return (root);
}
