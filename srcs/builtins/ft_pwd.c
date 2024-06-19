/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 16:49:27 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/19 16:57:24 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_putendl_fd(char *s, int fd)
{
	if (!s)
		return ;
	while (*s)
	{
		ft_putchar_fd(*s, fd);
		s ++;
	}
	ft_putchar_fd('\n', fd);
}

int	ft_pwd(void)
{
	char	buffer[1024];

	if (getcwd(buffer, 1024))
	{
		ft_putendl_fd(buffer, 1);
		return (0);
	}
	return (1);
}