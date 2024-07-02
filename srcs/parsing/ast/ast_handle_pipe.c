/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_handle_pipe.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 20:38:03 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/02 16:46:58 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	if_is_pipe(t_token **tok)
{
	//(*tok)->type == TOKEN_PIPE || 
	if ((*tok)->type == TOKEN_PIPE)
		return (1);
	else
		return (0);
}

void	while_in_handle_pipe(t_ast **current, t_ast **new_node, t_ast *save_operator)
{
	while ((save_operator != (*current)) &&((*current)->parent && *current))
		*current = (*current)->parent;
	(*new_node)->left = (*current)->right;
	(*new_node)->parent = *current;
	(*current)->right = (*new_node);
	while (*current && (*current)->right)
		(*current) = (*current)->right;
}

void	handle_pipe(t_token **tokens, t_ast **current, t_ast **root, t_ast **save_operator, t_ast **save_pipe)
{
	t_ast	*new_node;
	t_token	*temp;

	new_node = create_node((*tokens)->type, (*tokens)->value);
	if (!new_node)
		return ;
	if (!*save_operator)
	{
			new_node->left = *root;
			*save_operator = new_node;
			*current = new_node;
			*save_pipe = *current;
			printf("in if\n");
	}
	else
	{
		//(*root)->left = new_node;
		//printf("root->value %s | save_operator = %s", (*root)->value, (*save_operator)->value);
		new_node->left = (*save_operator)->right;
		(*save_operator)->right = new_node;
		new_node->parent = *save_operator;
		//*save_operator = new_node;
		*current = new_node;
		//*save_operator = *current;
		*save_pipe = *current;
		printf("save_pipe address in PIPE = %p\n", save_pipe);
	}
	free_token_and_next_in_ast(tokens, &temp);
}