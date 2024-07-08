/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_handle_and_or.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 19:52:14 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/08 14:20:59 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_and_or_root_priority(t_token **tokens, t_ast_ptr **list,
	int sub_shell)
{
	t_ast	*new_node;
	t_token	*temp;

	new_node = create_node(*tokens, sub_shell);
	if (!new_node)
		return ;
	new_node->left = (*list)->root;
	if ((*list)->root)
		((*list)->root)->parent = new_node;
	(*list)->root = new_node;
	(*list)->current = (*list)->root;
	(*list)->last_ope = (*list)->current;
	free_token_and_next(tokens, &temp);
}
