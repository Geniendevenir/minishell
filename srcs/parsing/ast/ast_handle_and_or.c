/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_handle_and_or.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 19:52:14 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/03 19:32:12 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_ope(t_token **tok)
{
	if ((*tok)->type == TOKEN_AND || (*tok)->type == TOKEN_OR)
		return (1);
	else
		return (0);
}

void	handle_and_or_root_priority(t_token **tokens, t_ast_ptr **list)
{
	t_ast	*new_node;
	t_token	*temp;

	new_node = create_node((*tokens)->type, (*tokens)->value);
	if (!new_node)
		return ;
	new_node->left = (*list)->root;
	if ((*list)->root)// TRES SUSPECT CAAAAAAAAAAAAAAAA if (root) pas *root
		((*list)->root)->parent = new_node;
	(*list)->root = new_node;
	(*list)->current = (*list)->root;
	(*list)->last_ope = (*list)->current;
	free_token_and_next_in_ast(tokens, &temp);
}
