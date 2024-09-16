/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 11:10:17 by Matprod           #+#    #+#             */
/*   Updated: 2024/09/15 18:26:13 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	sighandler(int signal)
{
	extern int	g_sig_int;

	if (signal == SIGINT)
	{
		g_sig_int = 1;
		write(1, "\n", 2);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	return ;
}

int	create_signal(void)
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
	a.sa_handler = SIG_IGN;
	sigemptyset(&a.sa_mask);
	if (sigaction(SIGTSTP, &a, NULL) != 0
		|| sigaction(SIGQUIT, &a, NULL) != 0)
		return (-1);
	return (0);
}

void	sighandler_exec(int signal)
{
	extern int	g_sig_int;

	if (signal == SIGINT)
	{
		g_sig_int = 2;
		write(1, "\n", 2);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (signal == (int)SIGQUIT)
	{
		g_sig_int = 1;
		write(2, "Quit: (core dumped)\n", 20);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	return ;
}

int	create_signal_exec(void)
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
	a.sa_handler = sighandler_exec;
	a.sa_flags = 0;
	sigemptyset(&a.sa_mask);
	if (sigaction(SIGINT, &a, NULL) != 0
		|| sigaction(SIGQUIT, &a, NULL) != 0)
		return (-1);
	a.sa_handler = SIG_IGN;
	sigemptyset(&a.sa_mask);
	if (sigaction(SIGTSTP, &a, NULL) != 0)
		return (-1);
	return (0);
}
