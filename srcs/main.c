/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 11:15:53 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/02 14:33:04 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_sig g_sig;

char    *minishell(t_all *p)
{
	rl_event_hook = event;
	init_signal(p->sig, 0);
	p->line = readline("\033[1;032mMinishell> \033[m");
	if (p->line == NULL)
    {
    	printf("exit\n");
		free(p->line);
		free_env(p->env);
		free(p->sig);
		free(p);
		rl_clear_history();
        exit(0);
    }
	if (g_sig.sig_int == 0) 
	{
		add_history(p->line);
	}
	g_sig.sig_int = 0;
	free(p->line);
	p->line = NULL;
	return (p->line);
	
}

//valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes
//--suppressions=./.readline.supp ./minishell

int main(int argc, char **argv, char **env) 
{
	t_all	*p;

	(void)argc;
	(void) **argv;
	p = init_env(env);
	if (!p || p == NULL)
		return (EXIT_FAILURE);
	print_env(p->env);
	while(p->line == NULL)
	{
    	minishell(p);
	}
	return (EXIT_SUCCESS);
}
