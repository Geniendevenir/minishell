/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_redirect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 12:31:35 by allan             #+#    #+#             */
/*   Updated: 2024/09/15 12:54:43 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		open_pipe(t_all *p, t_exec *exec, int option)
{
	if (p->curr_pipe == 0)
		open_first_pipe(p, exec, option);
	else if (p->curr_pipe > 0 && p->curr_pipe < p->max_pipe)
		open_middle_pipe(p, exec);
	else if (p->curr_pipe == p->max_pipe && exec->in && exec->in->type == TOKEN_PIPE)
		open_last_pipe(p, exec, option);
	return (0);
}

void	open_first_pipe(t_all *p, t_exec *exec, int option)
{
	int	fd_null;

	if (p->skip == 1 && exec->in && exec->in->type == TOKEN_PIPE)
	{
		fd_null = open("/dev/null", O_RDONLY);
		 if (fd_null < 0)
			return (pipe_error(1), exit(1));
		if (dup2(fd_null, STDIN_FILENO) < 0)
			return (pipe_close_fd(p, 1), pipe_error(2), exit(1));
		close(fd_null);
	}
	if (exec->out && exec->out->type == TOKEN_PIPE)
	{
		if (dup2(p->fd[p->curr_pipe][1], STDOUT_FILENO) < 0)
			return (pipe_close_fd(p, 1), pipe_error(3), exit(1));
	}
	close(p->fd[p->curr_pipe][0]);
	close(p->fd[p->curr_pipe][1]);
}

void	open_middle_pipe(t_all *p, t_exec *exec)
{
	if (exec->in && exec->in->type == TOKEN_PIPE)
	{
		if (dup2(p->fd[p->curr_pipe - 1][0], STDIN_FILENO) < 0)
				return (pipe_close_fd(p, 1), pipe_error(3), exit(1));
	}
	pipe_close_fd(p, 1);
	if (exec->out && exec->out->type == TOKEN_PIPE)
	{
		if (dup2(p->fd[p->curr_pipe][1], STDOUT_FILENO) < 0)
			return (pipe_close_fd(p, 1), pipe_error(3), exit(1));
	}
	close(p->fd[p->curr_pipe][0]);
	close(p->fd[p->curr_pipe][1]);
}

void	open_last_pipe(t_all *p, t_exec *exec, int option)
{
	int	fd_null;

	if (option == 1 && exec->in && exec->in->type == TOKEN_PIPE)
	{
		fd_null = open("/dev/null", O_RDONLY);
		 if (fd_null < 0)
		 	return (pipe_close_fd(p, 1), pipe_error(1), exit(1));
		if (dup2(fd_null, STDIN_FILENO) < 0)
		{
			close(fd_null);
			return (pipe_close_fd(p, 1), pipe_error(2), exit(1));
		}
		close(fd_null);
	}
	else
	{
		if (dup2(p->fd[p->curr_pipe - 1][0], STDIN_FILENO) < 0)
			return (pipe_close_fd(p, 1), pipe_error(3), exit(1));
		pipe_close_fd(p, 1);
	}
}
