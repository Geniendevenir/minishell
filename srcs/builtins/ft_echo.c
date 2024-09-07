/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 16:29:27 by Matprod           #+#    #+#             */
/*   Updated: 2024/09/07 15:13:47 by Matprod          ###   ########.fr       */
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
int echo_error(char **cmd)
{
	if (write(1, "\0", 1) == -1)
		if (errno == ENOSPC)
			return (ft_putendl_fd(
					"echo: write error: No space left on device", 2), 1);
	if (!cmd || !cmd[0])
	{
		write(1, "\n", 1);
		return (1);
	}
	return(0);
}

int	ft_echo(char **cmd)
{
	int		i;
	bool	option;

	option = false;
	i = 0;
	if (echo_error(cmd))
		return (EXIT_FAILURE);
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
