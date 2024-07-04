/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 18:41:34 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/03 21:09:44 by Matprod          ###   ########.fr       */
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
	while ((*list)->current && (*list)->current->parent)
		(*list)->current = (*list)->current->parent;
}

void	init_pointer_ast(t_ast_ptr **list)
{
	*list = malloc(sizeof(t_ast_ptr)); 
	if (!*list)
	{
		printf("erreur malloc\n");
		return ;
	}
	(*list)->last_pipe = NULL;
	(*list)->last_ope = NULL;
	(*list)->root = NULL;
	(*list)->current = NULL;
}



void	ope_pipe_redirect(t_token **tokens, t_ast_ptr **list)
{
	if (is_ope(tokens))
			handle_and_or_root_priority(tokens, list);
	else if (is_pipe(tokens))
			handle_pipe(tokens, list);
	else if (is_redirect(tokens))
		handle_redirect(tokens, list);
}

t_ast	*parse_expression(t_token **tokens)
{
	t_ast_ptr *list;
	t_ast	*current;
	t_token	*temp;

	init_pointer_ast(&list);
	while (*tokens)
	{
		if ((*tokens)->type == TOKEN_OPENPAR)
			handle_parenthesis_open(tokens, &list);
		else if ((*tokens)->type == TOKEN_CLOSEPAR)
			return (close_parenthesis(tokens, list->root));
		else if (is_ope(tokens) || is_pipe(tokens) || is_redirect(tokens))
			ope_pipe_redirect(tokens, &list);
		else if (if_cmd_or_option(tokens))
			handle_builtin_cmd_or_option(tokens, &list);
		else
			free_token_and_next_in_ast(tokens, &temp);
	}
	get_first_parent(&list);
	current = list->current;
	free(list);
	return (current);
}

t_ast	*parse_subexpression(t_token **tokens)
{
	return (parse_expression(tokens));
}
