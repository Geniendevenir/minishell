/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:11:24 by allan             #+#    #+#             */
/*   Updated: 2024/09/13 14:55:43 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_files(t_exec *exec)
{
	int	result;
	int	result2;

	result = open_filein(exec);
	/* printf("FILES:\n");
	printf("result = %d\n", result); */
	if (result == -1)
	{
		//printf("errno = %d\n", errno);
		if (errno == 2)
			error_executer(exec->in->value, 1);
		else
			error_executer(exec->in->value, 3);
		return (1); //correct
	}
	result2 = open_fileout(exec);
	//printf("result2 = %d\n", result2);
	if (result2 == -1)
	{
		error_executer(exec->in->value, 2);
		return (1); //correct
	}
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
	//open_pipe(p, exec);
	return (0);
}

int    open_filein(t_exec *exec)
{
	if (exec->in && exec->in->type != TOKEN_PIPE)
	{
		exec->filein = open(exec->in->value, O_RDONLY);
		if (exec->filein == -1)
		{
			exec->filein = 0;
			return (-1); // Error -> errno set
		}
		return (0); // Tout est OK
	}
	return (1); // NULL
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
		printf("pipe 1\n");
		if (p->skip == 1 && exec->in && exec->in->type == TOKEN_PIPE)
		{
			printf("SPACIAL DUP\n");
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
		printf("pipe 2\n");
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
		printf("pipe 3\n");
		if (option == 1 && exec->in && exec->in->type == TOKEN_PIPE)
		{
			printf("SPACIAL DUP\n");
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
		write(2, "1\n", 2);
		close(exec->filein);
		if (dup2(p->std_in, STDIN_FILENO) < 0)
			return (-1);
	}
	if (exec->fileout != 0)
	{
		write(2, "2\n", 2);
		close(exec->fileout);
		if (dup2(p->std_out, STDOUT_FILENO) < 0)
			return (-1);
	}
	return (0);
}
