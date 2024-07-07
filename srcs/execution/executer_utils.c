/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 18:39:42 by allan             #+#    #+#             */
/*   Updated: 2024/07/06 20:57:55 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exec	exec_init(t_exec exec)
{
	exec.pipe = 0;
	exec.redirectin = 0;
	exec.redirectout = 0;
	exec.in = NULL;
	exec.out = NULL;
	exec.command = NULL;
	return (exec);
}

void	exec_free(t_exec *exec)
{
	exec->pipe = 0;
	exec->redirectin = 0;
	exec->redirectout = 0;
	exec->in = NULL;
	exec->out = NULL;
	if (exec->command)
		free_array(exec->command);
	exec->command = NULL;
}

void	set_pipe(t_exec *exec, int set_pipe)
{
	if (set_pipe == 1)
		exec->pipe = 1;
	else if (set_pipe == 2)
		exec->pipe = 2;
}
