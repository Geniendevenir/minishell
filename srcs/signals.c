/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 11:10:17 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/08 13:47:00 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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


void sighandler(int signal)
{
	extern int sig_int;
	
	if (signal == SIGINT)
	{
		sig_int = 1;
		write(1, "\n", 2);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	return ;
}
/* 
void	sig_eof(int code, t_all *p)
{
	char	*save;

	(void) code;
	if (p->sig->p_status == 0 && rl_end == 0)
	{
		p->sig->sig_quit = 1;
		rl_on_new_line();
		rl_replace_line("end", 0);
		printf("\nexit\n");
		rl_done = 1;
	}
	else if (p->sig->p_status == 0)
	{
		save = ft_strdup(rl_line_buffer);
		if (save == NULL)
void	sig_eof(int code, t_all *p)
{
	char	*save;

	(void) code;
	if (p->sig->p_status == 0 && rl_end == 0)
	{
		p->sig->sig_quit = 1;
		rl_on_new_line();
		rl_replace_line("end", 0);
		printf("\nexit\n");
		rl_done = 1;
	}
	else if (p->sig->p_status == 0)
	{
		save = ft_strdup(rl_line_buffer);
		if (save == NULL)
			rl_on_new_line();
		rl_on_new_line();
		rl_replace_line(save, 0);
		rl_redisplay();
		free(save);
	}
	else if (p->sig->p_status == 2)
		p->sig->sig_quit = 1;
}
			rl_on_new_line();
		rl_on_new_line();
		rl_replace_line(save, 0);
		rl_redisplay();
		free(save);
	}
	else if (p->sig->p_status == 2)
		p->sig->sig_quit = 1;
} */


int	create_signal(void)
{
	struct termios old_termios;
	struct termios new_termios;
	struct sigaction a;
	
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
	if (sigaction(SIGTSTP, &a, NULL) != 0 || sigaction(SIGQUIT, &a, NULL) != 0)
	{
		return (-1);
	}
	return (0);
}
