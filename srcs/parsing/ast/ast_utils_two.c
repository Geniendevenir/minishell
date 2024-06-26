/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils_two.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 19:14:01 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/30 21:00:13 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	if_priorities(t_token **tok)
{
	if ((*tok)->type == 5 || (*tok)->type == 4)
		return (1);
	else
		return (0);
}

bool	if_is_redirect_and_pipe(t_token **tok)
{
	if ((*tok)->type == TOKEN_PIPE || (*tok)->type == WORD_FILEIN
		|| (*tok)->type == WORD_LIMITER || (*tok)->type == WORD_FILEOUT
		|| (*tok)->type == WORD_FILEOUT_APPEND)
		return (1);
	else
		return (0);
}

void	get_first_parent(t_ast **current)
{
	while (*current && (*current)->parent)
		*current = (*current)->parent;
}

void	handle_parenthesis_open(t_token **tokens, t_ast **current, t_ast **root)
{
	if ((*tokens)->type == TOKEN_OPENPAR)
	{
		*current = handle_open_parenthesis(tokens, *current);
		if (!*root)
			*root = *current;
	}
}

void	handle_builtin_option(t_token **tokens, t_ast **current, t_ast **root)
{
	if ((*tokens)->type == WORD_BUILTIN || (*tokens)->type == WORD_CMD
		|| (*tokens)->type == WORD_ABSPATH)
	{
		*current = handle_builtin_and_cmd(tokens, *current);
		if (!*root)
			*root = *current;
	}
	else if ((*tokens)->type == WORD_OPTION)
		*current = handle_option(tokens, *current);
}

bool	if_cmd_option(t_token **tokens)
{
	if ((*tokens)->type == WORD_BUILTIN || (*tokens)->type == WORD_CMD
		|| (*tokens)->type == WORD_ABSPATH || (*tokens)->type == WORD_OPTION)
		return (1);
	else
		return (0);
}
