/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 11:15:53 by Matprod           #+#    #+#             */
/*   Updated: 2024/09/15 13:04:56 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_sig_int = 0;

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
		minishell(p, env);
	}
	return (0);
}

char	*minishell(t_all *p, char **env)
{
	extern int	g_sig_int;
	t_ast		*current;

	rl_event_hook = event;
	p->line = readline("\033[1;032mMinishell> \033[m");
	if (g_sig_int == 1)
		p->exit_status = 127;
	if (p->line == NULL)
		return (ft_putstr("exit\n"), free_all(p), exit(0), NULL);
	if (p->sig->sig_quit == 0 && skip_whitespace(p->line))
	{
		p->error = parser(p->line, &p->ast, &p);
		if (p->error == 0)
		{
			current = p->ast;
			if (executer(p, current, env) == 1)
				return (free_ast(p->ast), free_all(p), exit(0), NULL);
			free_here_docs(p->here_doc);
			free_ast(p->ast);
		}
		add_history(p->line);
	}
	return (update_variable(p), p->line);
}
