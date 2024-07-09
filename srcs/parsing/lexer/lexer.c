/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 14:38:47 by allan             #+#    #+#             */
/*   Updated: 2024/07/09 13:06:41 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool lexer(char *cmd_line, t_token **token_list, int error)
{
	size_t	i;

	if (check_quotes(cmd_line) || check_semicolon(cmd_line))
	{
		if (check_quotes(cmd_line) == 1)
			error_lexer(ERROR_DQUOTES);
		else if (check_quotes(cmd_line) == 2)
			error_lexer(ERROR_SQUOTES);
		else if (check_semicolon(cmd_line) == 1)
			error_lexer(ERROR_SEMICOLON);
		else if (check_semicolon(cmd_line) == 2)
			error_lexer(ERROR_DUOSEMICOLON);
		token_free(token_list);
		return (1);
	}
	i = 0;
	while (i < ft_strlen(cmd_line))
	{
		error = tokenizer_one(cmd_line, &i, token_list);
		if (error != 0)
		{
			token_free(token_list);
			error_lexer(error);
			return (1);
		}
	}
	return (0);
}

int	tokenizer_one(const char *cmd_line, size_t *i, t_token **token_list)
{
	int	error;

	error = 0;
	if (is_whitespace(cmd_line[*i]))
		(*i)++;
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
	else
		error = tokenizer_two(cmd_line, i, token_list);
	return (error);
}

int	tokenizer_two(const char *cmd_line, size_t *i, t_token **token_list)
{
	int	error;

	error = 0;
	if (cmd_line[*i] == '&')
			error = and_token(cmd_line, i, token_list);
	else if (cmd_line[*i] == '<')
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
	else
		error = tokenizer_three(cmd_line, i, token_list);
	return (error);
	
}

int	tokenizer_three(const char *cmd_line, size_t *i, t_token **token_list)
{
	int	error;

	error = 0;
	if (last_heredoc(token_list) == 1)
		error = limiter_token(cmd_line, i, token_list);
	else
		error = lexical_token(cmd_line, i, token_list);
	return (error);
}
