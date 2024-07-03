/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_handle_and_or.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 19:52:14 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/02 12:40:19 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	if_token_and_or(t_token **tok)
{
	if ((*tok)->type == TOKEN_AND || (*tok)->type == TOKEN_OR)
		return (1);
	else
		return (0);
}

void	handle_and_or_root_priority(t_token **tokens, t_ast	**root,
	t_ast **current, t_ast **save_operator)
{
	t_ast	*new_node;
	t_token	*temp;

	new_node = create_node((*tokens)->type, (*tokens)->value);
	if (!new_node)
		return ;
	new_node->left = *root;
	if (root)
		(*root)->parent = new_node;
	*root = new_node;
	*current = *root;
	*save_operator = *current;
	free_token_and_next_in_ast(tokens, &temp);
}
