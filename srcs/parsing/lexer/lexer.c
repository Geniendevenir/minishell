/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 14:38:47 by allan             #+#    #+#             */
/*   Updated: 2024/06/14 16:31:38 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
		TO DO:
	0 - MODIFY STATE: OK
	1 - FIX NODE / CREATE PRINT TAB: OK
	2 - CREATE ERROR MANAGEMENT: OK
	3 - CHECK LEAKS: OK
	4 - FIRST SYNTAX CHECK
	5 - EXPAND
	6 - SECOND SYNTAX CHECK
*/

bool lexer(char *cmd_line, t_token **token_list)
{
	size_t	i;
	int		error;
	
	if (check_quotes(cmd_line) || check_semicolon(cmd_line))
	{
		token_free(token_list);
		write(2, "Error syntax\n", 13); // check la veritable erreur a renvoyer
		return (1);
	}
	i = 0;
	while (i < ft_strlen(cmd_line))
	{
		error = tokenizer(cmd_line, &i, token_list);
		if (error != 0)
		{
			token_free(token_list);
			error_lexer(error);
			return (1);
		}
	}
	return (0);
}

int	tokenizer(const char *cmd_line, size_t *i, t_token **token_list)
{
	int	error;
	
	error = 0;
	if (is_whitespace(cmd_line[*i]))
		error = whitespace_token(cmd_line, i, token_list);
	else if (cmd_line[*i] == '\"')
		error = env_dquotes_token(cmd_line, i, token_list);
	else if (cmd_line[*i] == '\'')
		error = squote_token(cmd_line, i, token_list);
	else if (cmd_line[*i] == '(')
		error = inpar_token(i, token_list);
	else if (cmd_line[*i] == ')')
		error = outpar_token(i, token_list);
	else if (cmd_line[*i] == '|')
	{
		if (cmd_line[*i + 1] == '|')
			error = or_token(i, token_list);
		else
			error = pipe_token(i, token_list);
	}
	else if (cmd_line[*i] == '&')
			error = and_token(cmd_line, i, token_list);
	else
		error = tokenizer_partwo(cmd_line, i, token_list);
	return (error);
}

int	tokenizer_partwo(const char *cmd_line, size_t *i, t_token **token_list)
{
	int	error;
	
	error = 0;
	if (cmd_line[*i] == '<')
	{
		if (cmd_line[*i + 1] == '<')
			error = heredoc_token(i, token_list);
		else
			error = inputre_token(i, token_list);
	}
	else if (cmd_line[*i] == '>')
	{
		if (cmd_line[*i + 1] == '>')
			error = outputapp_token(i, token_list);
		else
			error = outputre_token(i, token_list);
	}
	else if (cmd_line[*i] == '$')
		error = env_token(cmd_line, i, token_list);
	else if (cmd_line[*i] == '*' || is_wildcard(cmd_line, *i) == 0)
		error = wildcard_token(cmd_line, i, token_list);
	else
		error = lexical_token(cmd_line, i, token_list);
	return (error);
}
