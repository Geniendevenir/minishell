/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 16:29:27 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/21 11:59:23 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_echo(char *str)
{
	unsigned int	i;

	if (ft_strlen(str) < 2)
		return (false);
	else if (str[0] != '-')
		return (false);
	i = 0;
	while (str[++i])
		if (str[i] != 'n')
			return (false);
	return (true);
}

/* static int	conditions_echo(char *cmd)
{
	if (!(ft_strlen(cmd) == 2
			&& ((cmd[0] == '\"' && cmd[1] == '\"')
				|| (cmd[0] == '\'' && cmd[1] == '\''))))
		return (1);
	return (0);
} */

/*
** Our own echo function
** Usage : echo [option] [string]
** Options : -n
** -n : do not output the trailing newline
**
** @param cmd: array of strings containing the command and its arguments
**
** @return: EXIT_SUCCESS (0)
*/
int	ft_echo(char **cmd)
{
	int		i;
	bool	option;

	option = false;
	i = 0;
	if (write(1, "\0", 1) == -1)
		if (errno == ENOSPC)
			return (ft_putendl_fd(
					"echo: write error: No space left on device", 2), 1);
	if (!cmd || !cmd[0])
	{
		write(1, "\n", 1);
		return (0);
	}
	while (cmd[i] && check_echo(cmd[i]))
	{
		option = true;
		i++;
	}
	while (cmd[i])
	{
		write(1, cmd[i], ft_strlen(cmd[i]));
		if (cmd[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (option == false)
		write(1, "\n", 1);
	return (EXIT_SUCCESS);
}
