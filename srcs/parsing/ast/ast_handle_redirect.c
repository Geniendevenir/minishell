/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_handle_redirect.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:48:03 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/03 13:56:43 by Matprod          ###   ########.fr       */
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
		printf("save_pipe address in pipe = %p\n", save_pipe);
		while ((save_pipe) != (*current) &&((*current)->parent && *current))
			*current = (*current)->parent;
		(*new_node)->left = (*current)->right;
		(*new_node)->parent = *current;
		(*current)->right = (*new_node);
		while (*current && (*current)->right)
			(*current) = (*current)->right;
	}
	else if (save_operator)
	{	printf("save_pipe address if no pipe = %p\n", save_pipe);
		while ((save_operator) != (*current) &&((*current)->parent && *current))
			*current = (*current)->parent;
		(*new_node)->left = (*current)->right;
		(*new_node)->parent = *current;
		(*current)->right = (*new_node);
		while (*current && (*current)->right)
			(*current) = (*current)->right;
	}
}

void	handle_redirect(t_token **tokens, t_ast **current, t_ast **root, t_ast **save_operator, t_ast **save_pipe)
{
	t_ast	*new_node;
	t_token	*temp;
	
	new_node = create_node((*tokens)->type, (*tokens)->value);
	printf("IN REDIRECT :\n");
	if (!new_node)
		return ;
	if (!*root)
	{
		printf("if no root \n");
		*current = new_node;
		*root = *current;
	}
	else
	{
		if (*save_operator || save_pipe)
		{
			while_in_handle_redirect(current, &new_node, *save_operator,*save_pipe);
		}
		else
		{
			printf("if no operator and no pipe\n");
			new_node->left = *root;
			if (*root)
				(*root)->parent = new_node;
			*root = new_node;
		}
	}
	//new_node = *save_operator;
	printf("\n");
	free_token_and_next_in_ast(tokens, &temp);
}

