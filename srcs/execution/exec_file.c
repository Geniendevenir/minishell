/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:11:24 by allan             #+#    #+#             */
/*   Updated: 2024/09/15 15:38:50 by allan            ###   ########.fr       */
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

int	open_filein(t_exec *exec)
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

int	open_fileout(t_exec *exec)
{
	if (exec->out && exec->out->type != TOKEN_PIPE)
	{
		if (exec->out->type == WORD_FILEOUT)
		{
			exec->fileout = open(exec->out->value, O_TRUNC
					| O_CREAT | O_RDWR, 0000644);
			if (exec->fileout == -1)
				return (-1);
			return (0);
		}
		else if (exec->out->type == WORD_FILEOUT_APPEND)
		{
			exec->fileout = open(exec->out->value, O_APPEND
					| O_CREAT | O_RDWR, 0000644);
			if (exec->fileout == -1)
				return (-1);
			return (0);
		}
	}
	return (1);
}

int	close_files(t_exec *exec, t_all *p)
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
