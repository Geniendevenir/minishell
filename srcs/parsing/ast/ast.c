/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 18:41:34 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/05 17:19:37 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ope_pipe_redirect(t_token **tokens, t_ast_ptr **list, int subshell)
{
	if (is_ope(tokens))
		handle_and_or_root_priority(tokens, list, subshell);
	else if (is_pipe(tokens))
		handle_pipe(tokens, list, subshell);
	else if (is_redirect(tokens))
		handle_redirect(tokens, list, subshell);
}

void	free_list_ptr(t_ast_ptr **list, t_ast **temp_free, int option)
{
	if (option == 1)
	{
		*temp_free = (*list)->root;
		free((*list));
	}
	else if (option == 2)
	{
		*temp_free = (*list)->current;
		free((*list));
	}
}

t_ast	*parse_expression(t_token **tokens, int subshell)
{
	t_ast_ptr	*list;
	t_ast		*temp_free;
	t_token		*temp;

	init_pointer_ast(&list);
	while (*tokens)
	{
		if ((*tokens)->type == TOKEN_OPENPAR)
			handle_parenthesis_open(tokens, &list, subshell);
		else if ((*tokens)->type == TOKEN_CLOSEPAR)
		{
			free_list_ptr(&list, &temp_free, 1);
			return (close_parenthesis(tokens, temp_free));
		}
		else if (is_ope(tokens) || is_pipe(tokens) || is_redirect(tokens))
			ope_pipe_redirect(tokens, &list, subshell);
		else if (if_cmd_or_option(tokens))
			handle_builtin_cmd_or_option(tokens, &list, subshell);
		else
			free_token_and_next_in_ast(tokens, &temp);
	}
	get_first_parent(&list);
	free_list_ptr(&list, &temp_free, 2);
	return (temp_free);
}

t_ast	*parse_subexpression(t_token **tokens, int subshell)
{
	return (parse_expression(tokens, subshell + 1));
}
