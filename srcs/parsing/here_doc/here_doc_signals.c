/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_signals.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 21:19:55 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/08 22:41:46 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_signal_here(t_all **p)
{
	struct termios		old_termios;
	struct termios		new_termios;
	struct sigaction	a;

	if (tcgetattr(0, &old_termios) != 0)
		return (-1);
	new_termios = old_termios;
	new_termios.c_cc[VEOF] = 4;
	new_termios.c_cc[VSUSP] = 26;
	if (tcsetattr(0, TCSANOW, &new_termios))
		return (-1);
	a.sa_handler = sighandler;
	a.sa_flags = 0;
	sigemptyset(&a.sa_mask);
	if (sigaction(SIGINT, &a, NULL) != 0)
		return (-1);
	return (init_signal(0, p), 0);
}

int	signals_hdoc(int opt, t_all **p)
{
	extern int sig_int;
	
	if (opt == 0)
	{
		if (stop_signals() == -1 || create_signal_here(p) == -1)
			return (-1);
		return ((*p)->sig->p_status = 2, 0);
	}
	else if (opt == 1)
	{
		(*p)->sig->p_status = 1;
		if (stop_signals() == 1 || create_signal() == -1)
			return (-1);
		if (sig_int != 1)
			init_signal(0, p);
		return (0);
	}
	return (-1);
}
