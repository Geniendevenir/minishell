/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 18:41:34 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/01 16:35:33 by Matprod          ###   ########.fr       */
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

t_ast	*parse_expression(t_token **tokens)
{
	t_ast	*root;
	t_ast	*current;
	t_ast   *save_operator;
	t_token	*temp;

	save_operator = NULL;
	root = NULL;
	current = NULL;
	while (*tokens)
	{
		if ((*tokens)->type == TOKEN_OPENPAR)
			handle_parenthesis_open(tokens, &current, &root);
		else if ((*tokens)->type == TOKEN_CLOSEPAR)
			return (handle_close_parenthesis(tokens, root));
		else if (if_priorities(tokens))
		{
			root = handle_priorities(tokens, root);
			printf("address of last save_operator2 = %p | root = %p | current = %p\n", save_operator, root, current);
			current = root;
			printf("address of last save_operator2 = %p | root = %p | current = %p\n", save_operator, root, current);
			save_operator = current;
			printf("address of last save_operator2 = %p | root = %p | current = %p\n", save_operator, root, current);
		}
		else if (if_is_redirect_and_pipe(tokens))
		{
			printf("address of in redirect pipe save_operator = %p\n", save_operator);
			handle_redirect_or_pipe(tokens, &current, &root, save_operator);
		}
		else if (if_cmd_option(tokens))
			handle_builtin_option(tokens, &current, &root);
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
