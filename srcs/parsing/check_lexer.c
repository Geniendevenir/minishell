/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 17:23:10 by allan             #+#    #+#             */
/*   Updated: 2024/06/02 22:37:33 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool check_semicolon(char *cmd_line)
{
	int	i;

	i = 0;
	while (cmd_line[i])
	{
		if (cmd_line[i] == ';')
			return (1);
		i++;
	}
	return (0);
}

bool check_quotes(char *cmd_line)
{
	int i;
	
	i = 0;
	while(cmd_line[i])
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
				return (1);
		}
		i++;
	}
	return (0);
}
