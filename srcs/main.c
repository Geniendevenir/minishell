/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 11:15:53 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/15 19:17:01 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_sig	g_sig;

char	*minishell(t_all *p)
{
	rl_event_hook = event;
	p->line = readline("\033[1;032mMinishell> \033[m");
	if (p->line == NULL)
	{
		printf("exit\n");
		return (free(p->line), free_all(p), rl_clear_history(), exit(0), NULL);
	}
	if (p->sig->sig_int == 0)
	{
		parser(p->line, p->env);
		add_history(p->line);
	}
	p->sig->sig_int = 0;
	free(p->line);
	p->line = NULL;
	return (p->line);
}

//valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes
//--suppressions=./.readline.supp ./minishell

int	main(int argc, char **argv, char **env)
{
	t_all	*p;

	(void)argc;
	(void) **argv;
	p = init_all(env);
	if (!p || p == NULL)
		return (EXIT_FAILURE);
	while (p->line == NULL)
	{
		minishell(p);
	}
	return (EXIT_SUCCESS);
}
