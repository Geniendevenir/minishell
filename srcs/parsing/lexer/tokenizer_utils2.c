/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 16:43:01 by Matprod           #+#    #+#             */
/*   Updated: 2024/09/13 17:25:22 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	limit_word(char c, int option)
{
	if (option == 0)
	{
		if (c == '|' || c == '&' || c == '$' || c == '\'' || c == '\"'
			|| c == '<' || c == '>' || c == '(' || c == ')')
			return (1);
		if (is_whitespace(c) == 1)
			return (1);
	}
	else if (option == 1)
	{
		if (c == '$' || c == '?' || is_env(c, 1) == 0)
			return (1);
	}
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
