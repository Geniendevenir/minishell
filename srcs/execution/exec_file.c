/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:11:24 by allan             #+#    #+#             */
/*   Updated: 2024/09/15 12:04:07 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_files(t_all *p, t_exec *exec)
{
	int	result;
	int	result2;

	result = open_filein(exec);
	if (result == -1)
	{
		p->error = filein_error(p, exec);
		if (p->error != 2)
			return (p->error);
	}
	result2 = open_fileout(exec);
	if (result2 == -1)
	{
		if (p->max_pipe > 0)
			pipe_close_fd(p, 3);
		return (error_executer(exec->in->value, 2), 1);
	}
	fd_dup(exec, result, result2);
	p->exit_status = 0;
	return (0);
}

void	fd_dup(t_exec *exec, int result, int result2)
{
	if (result == 0)
	{
		dup2(exec->filein, STDIN_FILENO);
		close(exec->filein);
	}
	if (result2 == 0)
	{
		dup2(exec->fileout, STDOUT_FILENO);
		close(exec->fileout);
	}
}

int	filein_error(t_all *p, t_exec *exec)
{
	if (p->max_pipe > 0)
		pipe_close_fd(p, 3);
	if (errno == 2)
	{
		p->exit_status = 1;
		if (p->max_pipe > 0)
			return (error_executer(exec->in->value, 1), exit(0), 0);
		return (error_executer(exec->in->value, 1), 0);
	}
	else
		return (error_executer(exec->in->value, 3), 1);
	return (2);
}

int    open_filein(t_exec *exec)
{
	if (exec->in && exec->in->type != TOKEN_PIPE)
	{
		exec->filein = open(exec->in->value, O_RDONLY);
		if (exec->filein == -1)
		{
			exec->filein = 0;
			return (-1);
		}
		return (0);
	}
	return (1);
}

int    open_fileout(t_exec *exec)
{
	if (exec->out && exec->out->type != TOKEN_PIPE)
	{
		if (exec->out->type == WORD_FILEOUT)
		{
			exec->fileout = open(exec->out->value, O_TRUNC | O_CREAT | O_RDWR, 0000644);
			if (exec->fileout == -1)
				return (-1);
			return (0);
		}
		else if(exec->out->type == WORD_FILEOUT_APPEND)
		{
			exec->fileout = open(exec->out->value, O_APPEND | O_CREAT | O_RDWR, 0000644);
			if (exec->fileout == -1)
				return (-1);
			return (0);
		}
	}
	return (1);
}

int		open_pipe(t_all *p, t_exec *exec, int option)
{
	int	fd_null;
	if (p->curr_pipe == 0)
	{
		if (p->skip == 1 && exec->in && exec->in->type == TOKEN_PIPE)
		{
			fd_null = open("/dev/null", O_RDONLY);
			 if (fd_null < 0)
			{
				perror("Error opening /dev/null");
				exit(1);
			}
			if (dup2(fd_null, STDIN_FILENO) < 0)
			{
				perror("Error redirecting stdin to /dev/null");
				close(fd_null);
				exit(1);
			}
			close(fd_null);
		}
		if (exec->out && exec->out->type == TOKEN_PIPE)
			dup2(p->fd[p->curr_pipe][1], STDOUT_FILENO);
		close(p->fd[p->curr_pipe][0]);
		close(p->fd[p->curr_pipe][1]);
	}
	else if (p->curr_pipe > 0 && p->curr_pipe < p->max_pipe)
	{
		if (exec->in && exec->in->type == TOKEN_PIPE)
			dup2(p->fd[p->curr_pipe - 1][0], STDIN_FILENO);
		pipe_close_fd(p, 1);
		if (exec->out && exec->out->type == TOKEN_PIPE)
			dup2(p->fd[p->curr_pipe][1], STDOUT_FILENO);
		close(p->fd[p->curr_pipe][0]);
		close(p->fd[p->curr_pipe][1]);
	}
	else if (p->curr_pipe == p->max_pipe && exec->in && exec->in->type == TOKEN_PIPE)
	{
		if (option == 1 && exec->in && exec->in->type == TOKEN_PIPE)
		{
			fd_null = open("/dev/null", O_RDONLY);
			 if (fd_null < 0)
			{
				perror("Error opening /dev/null");
				exit(1);
			}
			if (dup2(fd_null, STDIN_FILENO) < 0)
			{
				perror("Error redirecting stdin to /dev/null");
				close(fd_null);
				exit(1);
			}
			close(fd_null);
		}
		else
		{
			dup2(p->fd[p->curr_pipe - 1][0], STDIN_FILENO);
			pipe_close_fd(p, 1);
		}
	}
	return (0);
}

int		close_files(t_exec *exec, t_all *p)
{
	if (exec->filein != 0)
	{
		close(exec->filein);
		if (dup2(p->std_in, STDIN_FILENO) < 0)
			return (error_executer(NULL, 6), -1);
	}
	if (exec->fileout != 0)
	{
		close(exec->fileout);
		if (dup2(p->std_out, STDOUT_FILENO) < 0)
			return (error_executer(NULL, 6), -1);
	}
	return (0);
}
