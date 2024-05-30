/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 11:15:53 by Matprod           #+#    #+#             */
/*   Updated: 2024/05/30 16:26:19 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_sig g_sig;


void	init_signal(int nb)
{
	g_sig.sig_quit = nb;
	g_sig.sig_int = nb;
	g_sig.p_status = nb;
}

void sighandler(int signal)
{
    if (signal== SIGINT)
    {
        g_sig.sig_int = 1;
		if (g_sig.p_status == 0)
		{
			g_sig.cmd_stat = 130;
			rl_done = 1;
		}
    }
	if (signal== SIGTERM)
    {
        printf("\nExiting shell...\n");
        exit(0);
    }
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
	if (sigaction(SIGINT, &a, NULL) != 0 || sigaction(SIGTSTP, &a, NULL) != 0
		|| sigaction(SIGQUIT, &a, NULL) != 0 || sigaction(SIGTERM, &a, NULL) != 0)
		return (-1);
	return (0);
}

char    *minishell()
{
	init_signal(0);
	g_sig.line = readline("\033[1;032mMinishell> \033[m");
	if(g_sig.sig_int == 1)
	{
		
	}
	if (g_sig.line == NULL)
    {
    	printf("\nExiting shell...\n");
        exit(0);
    }
	if (g_sig.line && *g_sig.line) 
	{
		add_history(g_sig.line);
	}
	free(g_sig.line);
	g_sig.line = NULL;
	return (g_sig.line);
	
}
int main() 
{
	create_signal();
	g_sig.line = NULL;
	while(g_sig.line == NULL)
	{
    	minishell();
	}
	return (EXIT_SUCCESS);
}
