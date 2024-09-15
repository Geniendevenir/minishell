/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:42:50 by allan             #+#    #+#             */
/*   Updated: 2024/09/15 11:45:50 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipe_close_fd(t_all *p, int option)
{
	int	i;

	i = 0;
	if (option == 1)
	{
		while (i <= p->curr_pipe - 1)
		{
			close(p->fd[i][0]);
			close(p->fd[i][1]);
			i++;
		}
	}
	else if (option == 2)
	{
		while (i < p->max_pipe)
		{
			close(p->fd[i][0]);
			close(p->fd[i][1]);
			i++;
		}
	}
	else if (option == 3)
		pipe_close_fd_two(p);
}

void	pipe_close_fd_two(t_all *p)
{
	int	i;

	i = 0;
	while (i <= p->curr_pipe)
	{
		close(p->fd[i][0]);
		close(p->fd[i][1]);
		i++;
	}
}

void	pipe_free(t_all *p, t_exec *exec, int *pid)
{
	pipe_free_exec(&exec); //add close input/output
	if (p->fd)
		free(p->fd);
	if (pid)
		free(pid);
	p->max_pipe = 0;
	p->curr_pipe = 0;
}

void	pipe_free_exec(t_exec **exec)
{
	t_exec	*current;
	t_exec	*tmp;

	if (!exec || !(*exec))
		return ;
	current = *exec;
	while (current)
	{
		if (!current->next)
		{
			if (current)
			{
				exec_free(current);
				free(current);
			}
			break;
		}
		tmp = current->next;
		if (current)
		{
			exec_free(current);
			free(current);
		}
		current = tmp;
	}
}
