/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_handle_redirect.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:48:03 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/08 14:20:59 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	while_in_handle_redirect(t_ast_ptr **list, t_ast **new_node)
{
	if ((*list)->last_pipe)
	{
		while (((*list)->last_pipe) != (*list)->current
			&& ((*list)->current->parent && (*list)->current))
			(*list)->current = (*list)->current->parent;
		(*new_node)->left = (*list)->current->right;
		(*new_node)->parent = (*list)->current ;
		(*list)->current->right = (*new_node);
		while ((*list)->current && (*list)->current->right)
			(*list)->current = (*list)->current->right;
	}
	else if ((*list)->last_ope)
	{
		while (((*list)->last_ope) != (*list)->current
			&& ((*list)->current->parent && (*list)->current))
			(*list)->current = (*list)->current->parent;
		(*new_node)->left = (*list)->current->right;
		(*new_node)->parent = (*list)->current;
		(*list)->current->right = (*new_node);
		while ((*list)->current && (*list)->current->right)
			(*list)->current = (*list)->current->right;
	}
}

void	handle_redirect(t_token **tokens, t_ast_ptr **list, int sub_shell)
{
	t_ast	*new_node;
	t_token	*temp;

	new_node = create_node(*tokens, sub_shell);
	if (!new_node)
		return ;
	if (!(*list)->root)
	{
		(*list)->current = new_node;
		(*list)->root = (*list)->current;
	}
	else
	{
		if ((*list)->last_ope || (*list)->last_pipe)
			while_in_handle_redirect(list, &new_node);
		else
		{
			new_node->left = (*list)->root;
			if ((*list)->root)
				((*list)->root)->parent = new_node;
			(*list)->current = new_node;
			(*list)->root = (*list)->current;
		}
	}
	free_token_and_next(tokens, &temp);
}
