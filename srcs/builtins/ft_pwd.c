/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 16:49:27 by Matprod           #+#    #+#             */
/*   Updated: 2024/09/14 23:02:29 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(char *option)
{
	char	buffer[1024];
	int		i;

	i = 0;
	if (option)
	{
		if (option[i] == '-')
		{
			while (option[i] && option[i] == '-')
				i++;
			if (option[i] && ft_strlen(option) >= 2)
			{
				write(1, "bash: pwd: ", 11);
				write(1, &option[0], 1);
				write(1, &option[1], 1);
				write(1, ": invalid option\n", 17);
				return (2);
			}
		}
	}
	if (getcwd(buffer, 1024) == NULL)
		return (1);
	ft_putendl_fd(buffer, 1);
	return (0);
}
