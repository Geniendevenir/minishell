/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:42:50 by allan             #+#    #+#             */
/*   Updated: 2024/09/09 20:01:32 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipe_close_fd(t_all *p)
{
	if (p->curr_pipe > 0)
	{
		close(p->fd[p->curr_pipe - 1][0]);
		close(p->fd[p->curr_pipe - 1][1]);
	}
}

void	pipe_free(t_all *p, t_exec *exec)
{
	pipe_free_exec(&exec); //add close input/output
	//pipe_free_fd(p);
	if (p->fd)
		free(p->fd);
	p->max_pipe = 0;
	p->curr_pipe = 0;
}

void	pipe_free_exec(t_exec **exec)
{
	t_exec	*current;
	t_exec	*tmp;

	if (!exec)
		return ;
	current = *exec;
	while (current)
	{
		tmp = current->next;
		exec_free(current);
		free(current);
		current = tmp;
	}
}

void	pipe_free_fd(t_all *p)
{
	int	i;

	i = 0;
	if (!p->fd)
		return ;
	while (i < p->max_pipe)
	{
		close(p->fd[i][0]);
		close(p->fd[i][1]);
		i++;
	}
	free(p->fd);
	p->fd = NULL;
}
