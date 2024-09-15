/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 18:39:42 by allan             #+#    #+#             */
/*   Updated: 2024/09/15 11:41:50 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_init(t_exec *exec)
{
	exec->pipe = 0;
	exec->redirectin = 0;
	exec->redirectout = 0;
	exec->filein = 0;
	exec->fileout = 0;
	exec->in = NULL;
	exec->out = NULL;
	exec->command = NULL;
	exec->path = NULL;
	exec->next = NULL;
}

void		exec_free(t_exec *exec)
{
	exec->pipe = 0;
	exec->redirectin = 0;
	exec->redirectout = 0;
	exec->filein = 0;
	exec->fileout = 0;
	exec->in = NULL;
	exec->out = NULL;
	if (exec->command)
		free_array(exec->command);
	if (exec->path)
		free(exec->path);
	exec->command = NULL;
}

void	set_pipe(t_all *p, t_exec *exec)
{
	if (p->max_pipe > 0)
	{
		if (p->curr_pipe == 0)
			exec->pipe = 1;
		else if (p->curr_pipe == p->max_pipe)
			exec->pipe = 3;
		else
			exec->pipe = 2;	
	}
}

int		array_size(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}
