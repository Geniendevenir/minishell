/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 18:41:34 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/03 18:41:49 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_ast_ptr
{
	t_ast	*root;
	t_ast	*current;
	t_ast	*last_ope;
	t_ast	*last_pipe;
}	t_ast_ptr;


void	free_token_and_next_in_ast(t_token **tokens, t_token **temp)
{
	*temp = *tokens;
	*tokens = (*tokens)->next;
	free((*temp)->value);
	free((*temp));
}

void	get_first_parent(t_ast **current)
{
	while (*current && (*current)->parent)
		*current = (*current)->parent;
}

/* void	init_pointer_ast(t_ast_ptr **list)
{
	(*list)->save_pipe = NULL;
	(*list)->save_operator = NULL;
	(*list)->root = NULL;
	(*list)->current = NULL;
} */

void	init_pointer_ast(t_ast **current, t_ast **save_pipe, t_ast **save_operator, t_ast **root)
{
	*save_pipe = NULL;
	*save_operator = NULL;
	*root = NULL;
	*current = NULL;
}

void	ope_pipe_redirect(t_token **tokens, t_ast **current,
	t_ast **root, t_ast **save_operator, t_ast **last_pipe)
{
	if (is_ope(tokens))
			handle_and_or_root_priority(tokens, root, current, save_operator);
	else if (is_pipe(tokens))
			handle_pipe(tokens, current, root, save_operator, last_pipe);
	else if (is_redirect(tokens))
		handle_redirect(tokens, current, root, save_operator, last_pipe);
}

t_ast	*parse_expression(t_token **tokens)
{
	//t_ast_ptr *list;
	t_ast	*root;
	t_ast	*current;
	t_ast	*last_ope;
	t_ast	*last_pipe; 
	t_token	*temp;

	init_pointer_ast(&current, &last_pipe, &last_ope, &root);
	while (*tokens)
	{
		if ((*tokens)->type == TOKEN_OPENPAR)
			handle_parenthesis_open(tokens, &current, &root);
		else if ((*tokens)->type == TOKEN_CLOSEPAR)
			return (close_parenthesis(tokens, root));
		else if (is_ope(tokens) || is_pipe(tokens) || is_redirect(tokens))
			ope_pipe_redirect(tokens, &current, &root, &last_ope, &last_pipe);
		else if (if_cmd_or_option(tokens))
			handle_builtin_cmd_or_option(tokens, &current, &root);
		else
			free_token_and_next_in_ast(tokens, &temp);
	}
	get_first_parent(&current);
	return (current);
}

t_ast	*parse_subexpression(t_token **tokens)
{
	return (parse_expression(tokens));
}
