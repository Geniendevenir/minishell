/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 11:15:53 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/01 20:07:24 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_sig g_sig;

char    *minishell()
{
	rl_event_hook = event;
	init_signal(0);
	g_sig.line = readline("\033[1;032mMinishell> \033[m");
	if (g_sig.line == NULL)
    {
    	printf("Exit ctrl D\n");
		free(g_sig.line);
		rl_clear_history();
        exit(0);
    }
	if (g_sig.sig_int == 0) 
	{
		add_history(g_sig.line);
	}
	g_sig.sig_int = 0;
	free(g_sig.line);
	g_sig.line = NULL;
	return (g_sig.line);
	
}


//valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes
//--suppressions=./.readline.supp ./minishell


int main(int argc, char **argv, char **env) 
{
	t_env *p_env;
	
	(void)argc;
	(void) **argv;
	p_env = NULL;
	p_env = init_env(p_env, env);
	g_sig.line = NULL;
	print_env(p_env);
	while(g_sig.line == NULL)
	{
    	minishell();
	}
	return (EXIT_SUCCESS);
}
