/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:11:24 by allan             #+#    #+#             */
/*   Updated: 2024/08/01 10:59:16 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int    open_filein(t_exec *exec)
{
	if (exec->in)
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
	if (exec->out)
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

int	open_files(t_exec *exec)
{
	int	result;
	int	result2;
	
	//Add rules for pipe
	result = open_filein(exec);
	printf("FILES:\n");
	printf("result = %d\n", result);
	if (result == -1)
	{
		printf("errno = %d\n", errno);
		if (errno == 2)
			error_executer(exec->in->value, 1);
		else
			error_executer(exec->in->value, 3);
		return (1); //correct
	}
	result2 = open_fileout(exec);
	printf("result2 = %d\n", result2);
	if (result2 == -1)
	{
		error_executer(exec->in->value, 2);
		return (1); //correct
	}
	if (result == 0)
		dup2(exec->filein, STDIN_FILENO);
	if (result2 == 0)
		dup2(exec->fileout, STDOUT_FILENO);
	return (0);
}

int		close_files(t_exec *exec, int std_in, int std_out)
{
	if (exec->filein != 0)
	{
		close(exec->filein);
		if (dup2(std_in, STDIN_FILENO) < 0)
			return (-1);
	}
	if (exec->fileout != 0)
	{
		close(exec->fileout);
		if (dup2(std_out, STDOUT_FILENO) < 0)
			return (-1);
	}
	return (0);
}