/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_handle_redirect.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:48:03 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/02 16:50:13 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	if_is_redirect(t_token **tok)
{
	//(*tok)->type == TOKEN_PIPE || 
	if ((*tok)->type == TOKEN_PIPE || (*tok)->type == WORD_FILEIN
		|| (*tok)->type == WORD_LIMITER || (*tok)->type == WORD_FILEOUT
		|| (*tok)->type == WORD_FILEOUT_APPEND)
		return (1);
	else
		return (0);
}

void	while_in_handle_redirect(t_ast **current, t_ast **new_node, t_ast *save_operator, t_ast *save_pipe)
{
	if (save_pipe)
	{
		printf("save_pipe address in REDIRECT = %p\n", save_pipe);
		while ((save_pipe) != (*current) &&((*current)->parent && *current))
			*current = (*current)->parent;
		(*new_node)->left = (*current)->right;
		(*new_node)->parent = *current;
		(*current)->right = (*new_node);
	}
	else
	{
		while ((save_operator) != (*current) &&((*current)->parent && *current))
			*current = (*current)->parent;
		(*new_node)->left = (*current)->right;
		(*new_node)->parent = *current;
		(*current)->right = (*new_node);
	}
	while (*current && (*current)->right)
		(*current) = (*current)->right;
}

void	handle_redirect(t_token **tokens, t_ast **current, t_ast **root, t_ast **save_operator, t_ast **save_pipe)
{
	t_ast	*new_node;
	t_token	*temp;
	
	new_node = create_node((*tokens)->type, (*tokens)->value);
	if (!new_node)
		return ;
	if (!*root)
	{
		*current = new_node;
		*root = *current;
	}
	else
	{
		if (!*save_operator)
		{
			new_node->left = *root;
			if (*root)
				(*root)->parent = new_node;
			*root = new_node;
		}
		else
			while_in_handle_redirect(current, &new_node, *save_operator,*save_pipe);
	}
	new_node = *save_operator;
	free_token_and_next_in_ast(tokens, &temp);
}

