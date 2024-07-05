/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 19:12:00 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/05 14:33:24 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_token_and_next_in_ast(t_token **tokens, t_token **temp)
{
	*temp = *tokens;
	*tokens = (*tokens)->next;
	free((*temp)->value);
	free((*temp));
}

void	get_first_parent(t_ast_ptr **list)
{
	while ((*list)->current->parent)
		(*list)->current = (*list)->current->parent;
}

void	init_pointer_ast(t_ast_ptr **list)
{
	*list = malloc(sizeof(t_ast_ptr));
	if (!*list)
		return ;
	(*list)->last_pipe = NULL;
	(*list)->last_ope = NULL;
	(*list)->root = NULL;
	(*list)->current = NULL;
}
