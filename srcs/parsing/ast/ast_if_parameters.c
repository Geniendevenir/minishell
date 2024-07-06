/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_if_parameters.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 13:23:01 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/06 13:31:08 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	if_cmd_or_option(t_token **tokens)
{
	if ((*tokens)->type == WORD_BUILTIN || (*tokens)->type == WORD_CMD
		|| (*tokens)->type == WORD_ABSPATH || (*tokens)->type == WORD_OPTION)
		return (1);
	else
		return (0);
}

bool	is_redirect(t_token **tok)
{
	if ((*tok)->type == WORD_FILEIN
		|| (*tok)->type == WORD_LIMITER || (*tok)->type == WORD_FILEOUT
		|| (*tok)->type == WORD_FILEOUT_APPEND)
		return (1);
	else
		return (0);
}

bool	is_redirect_enum(enum s_type word)
{
	if (word == WORD_FILEIN
		|| word == WORD_LIMITER || word == WORD_FILEOUT
		|| word == WORD_FILEOUT_APPEND)
		return (1);
	else
		return (0);
}

bool	is_pipe(t_token **tok)
{
	if ((*tok)->type == TOKEN_PIPE)
		return (1);
	else
		return (0);
}

bool	is_ope(t_token **tok)
{
	if ((*tok)->type == TOKEN_AND || (*tok)->type == TOKEN_OR)
		return (1);
	else
		return (0);
}
