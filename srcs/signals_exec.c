/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 15:27:18 by Matprod           #+#    #+#             */
/*   Updated: 2024/09/15 15:37:52 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_handler_child(int sig)
{
	extern int	g_sig_int;

	if (sig == SIGINT)
		g_sig_int = 1;
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