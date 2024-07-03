/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 18:41:34 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/03 13:26:16 by Matprod          ###   ########.fr       */
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

void	get_first_parent(t_ast **current)
{
	while (*current && (*current)->parent)
		*current = (*current)->parent;
}

t_ast	*parse_expression(t_token **tokens)
{
	t_ast	*root;
	t_ast	*current;
	t_ast   *save_operator;
	t_ast	*save_pipe;
	t_token	*temp;

	save_pipe = NULL;
	save_operator = NULL;
	root = NULL;
	current = NULL;
	while (*tokens)
	{
		if ((*tokens)->type == TOKEN_OPENPAR)
			handle_parenthesis_open(tokens, &current, &root);
		else if ((*tokens)->type == TOKEN_CLOSEPAR)
			return (close_parenthesis(tokens, root));
		else if (if_token_and_or(tokens))
			handle_and_or_root_priority(tokens, &root, &current, &save_operator);
		else if ((*tokens)->type == TOKEN_PIPE)
		{
			handle_pipe(tokens, &current, &root, &save_operator, &save_pipe);
		}
		else if (if_is_redirect(tokens))
		{
			//printf("address of in redirect save_operator = %p\n", save_operator);
			handle_redirect(tokens, &current, &root, &save_operator, &save_pipe);
			if(current && root && save_operator)
				printf("current->value = %s | save_operator address = %p | root->value = %s\n", current->value, save_operator, root->value);
		}
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
