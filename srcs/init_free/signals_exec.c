/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 16:35:50 by allan             #+#    #+#             */
/*   Updated: 2024/09/15 18:28:29 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	event(void)
{
	return (42);
}

int	stop_signals(void)
{
	if (signal(SIGINT, SIG_IGN) == SIG_ERR
		|| signal(SIGQUIT, SIG_IGN) == SIG_ERR
		|| signal(SIGTSTP, SIG_IGN) == SIG_ERR)
		return (-1);
	return (0);
}

void	sig_handler_child(int sig)
{
	extern int	g_sig_int;

	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		g_sig_int = 2;
	}
	else if (sig == SIGQUIT)
	{
		g_sig_int = 1;
		write(1, "Quit: (core dumped)\n", 20);
	}
}

void	setup_signal_handlers(void (*int_)(int), void (*quit_)(int))
{
	rl_catch_signals = 1;
	signal(SIGINT, int_);
	signal(SIGQUIT, quit_);
}
