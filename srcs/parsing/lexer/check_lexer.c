/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 17:23:10 by allan             #+#    #+#             */
/*   Updated: 2024/07/11 16:47:03 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_semicolon(char *cmd_line)
{
	int	i;

	i = 0;
	while (cmd_line[i])
	{
		if (cmd_line[i] == '\"')
			i = skip_quotes(cmd_line, i, 0);
		else if (cmd_line[i] == '\'')
			i = skip_quotes(cmd_line, i, 1);
		if (cmd_line[i] == ';')
		{
			if (cmd_line[i + 1] == ';')
				return (2);
			return (1);
		}
		i++;
	}
	return (0);
}

int	check_quotes(char *cmd_line)
{
	int	i;

	i = 0;
	while (cmd_line[i])
	{
		if (cmd_line[i] == '\"')
		{
			i++;
			while (cmd_line[i] && cmd_line[i] != '\"')
				i++;
			if (!cmd_line[i])
				return (1);
		}
		else if (cmd_line[i] == '\'')
		{
			i++;
			while (cmd_line[i] && cmd_line[i] != '\'')
				i++;
			if (!cmd_line[i])
				return (2);
		}
		i++;
	}
	return (0);
}

bool	check_env_dquotes(const char *cmd_line, const int *i)
{
	int	j;

	j = *i + 1;
	while (cmd_line[j] && cmd_line[j] != '\"')
	{
		if (cmd_line[j] == '$')
			return (0);
		j++;
	}
	return (1);
}

int	skip_quotes(const char *cmd_line, int i, int option)
{
	i++;
	if (option == 0)
	{
		while (cmd_line[i] && cmd_line[i] != '\"')
			i++;
	}
	else if (option == 1)
	{
		while (cmd_line[i] && cmd_line[i] != '\'')
			i++;
	}
	return (i);
}
