/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:19:15 by allan             #+#    #+#             */
/*   Updated: 2024/07/01 16:53:05 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_whitespace(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

bool	is_env(char c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_'
		|| c == '?')
		return (0);
	return (1);
}

bool	is_valid_env(char c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '_') 
		|| (c >= '0' && c <= '9'))
		return (0);
	return (1);
}

bool	is_word(char c)
{
	if (c == '|' || c == '&' || c == '$' || c == '\'' || c == '\"'
		|| c == '<' || c == '>' || c == '(' || c == ')')
		return (1);
	if (is_whitespace(c) == 1)
		return (1);
	return (0);
}

bool	is_wildcard(const char *cmd_line, int i)
{
	while (cmd_line[i] && (is_word(cmd_line[i]) == 0 || cmd_line[i] == '*'))
	{
		if (cmd_line[i] == '*')
			return (0);
		i++;
	}
	return (1);
}

bool	is_freeable(char *value, int option)
{
	if (option == 2)
		return (1);
	if (option == 1)
	{
		if (!ft_strcmp(value, " ") || !ft_strcmp(value, ")")
			|| !ft_strcmp(value, "(") || !ft_strcmp(value, "<"))
			return (1);
		if (!ft_strcmp(value, "||") || !ft_strcmp(value, "|")
			|| !ft_strcmp(value, "&&") || !ft_strcmp(value, "<<"))
			return (1);
		if (!ft_strcmp(value, ">>") || !ft_strcmp(value, ">")
			|| !ft_strcmp(value, "?"))
			return (1);
	}
	return (0);
}
