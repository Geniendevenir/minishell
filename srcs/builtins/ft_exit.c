/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 12:53:13 by allan             #+#    #+#             */
/*   Updated: 2024/09/14 23:02:52 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	full_digit(char *str)
{
	int	i;

	i = -1;
	if (str)
	{
		if (ft_strlen(str) > 1 && (str[0] == '-' || str[0] == '+'))
			++i;
		while (str[++i])
		{
			if (!ft_isdigit(str[i]))
				return (false);
		}
	}
	return (true);
}

void	free_exit(t_all *p)
{
	close(p->std_in);
	close(p->std_out);
	free(p->line);
	free_here_docs(p->here_doc);
	free_env(p->env);
	free(p->sig);
	free_ast(p->ast);
	free(p);
	rl_clear_history();
}

int	ft_exit(t_all **p, t_exec *exec, char **cmd)
{
	int	nb;

	if (array_size(cmd) > 1 && full_digit(cmd[1]) && cmd[2])
		return (ft_putstr_fd("exit\n", 2),
			ft_putendl_fd("bash : exit: too many arguments", 2), 1);
	ft_putstr_fd("exit\n", 2);
	if (cmd[1])
	{
		if (!full_digit(cmd[1]))
		{
			ft_putstr_fd("bash: exit: ", 2);
			ft_putstr_fd(cmd[1], 2);
			ft_putendl_fd(": numeric argument required", 2);
			nb = 2;
		}
		else
			nb = (unsigned char)ft_atoi(cmd[1]);
	}
	else
		nb = 0;
	exec_free(exec);
	free_exit(*p);
	exit(nb);
	return (42);
}
