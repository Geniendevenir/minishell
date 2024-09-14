/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piper.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 17:46:20 by allan             #+#    #+#             */
/*   Updated: 2024/09/14 23:42:24 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	piper(t_all *p, t_ast *current, char **env)
{
	t_exec	*exec;
	int		*pid;

	pid = NULL;
	exec = malloc(sizeof(t_exec));
	if (!exec)
		return (error_executer(NULL, 4), 1);
	exec_init(exec);
	if (pipe_parser(p, current, &exec, 0) == 1)
		return (error_executer(NULL, 4), pipe_free(p, exec, pid), 1);
	if (pipe_analyser(p, exec) == 1)
		return (pipe_free(p, exec, pid), 0);
	p->curr_pipe = 0;
	if (pipe_allocation(p, &pid) == 1)
		return (error_executer(NULL, 4), pipe_free(p, exec, pid), 1);
	if (pipe_exec(p, &exec, env, pid) == 1)
		return (pipe_free(p, exec, pid), 1);
	return (pipe_free(p, exec, pid), 0);
}
