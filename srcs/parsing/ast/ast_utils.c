/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 19:12:00 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/07 20:09:34 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*create_node(t_token *token, int subshell)
{
	t_ast	*node;

	node = (t_ast *)malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = token->type;
	node->state = token->state;
	node->value = ft_strdup(token->value);
	if (!node->value)
		return (NULL);
	node->subshell = subshell;
	node->exit_state = 0;
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;
	return (node);
}

void	free_token_and_next_in_ast(t_token **tokens, t_token **temp)
{
	*temp = *tokens;
	*tokens = (*tokens)->next;
	free((*temp)->value);
	free((*temp));
}

void	get_first_parent(t_ast_ptr **list)
{
	while ((*list)->current && (*list)->current->parent)
		(*list)->current = (*list)->current->parent;
}

void	init_pointer_ast(t_ast_ptr **list)
{
	*list = malloc(sizeof(t_ast_ptr));
	if (!*list)
		return ;
	(*list)->last_pipe = NULL;
	(*list)->last_ope = NULL;
	(*list)->last_cmd = NULL;
	(*list)->root = NULL;
	(*list)->current = NULL;
}
