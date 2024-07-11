/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:11:24 by allan             #+#    #+#             */
/*   Updated: 2024/07/11 17:11:42 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int    open_filein(t_exec *exec)
{
	if (exec->in)
	{
		exec->filein = open(exec->in->value, O_RDONLY);
		if (exec->filein == -1)
			return (-1); // A fail a open
		else if (exec->filein == 2)
			return (2); // N'existe pas
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
			exec->fileout = open(exec->in->value, O_TRUNC | O_CREAT | O_RDWR, 0000644);
			if (exec->fileout == -1)
				return (-1);
			return (0);
		}
		else if(exec->out->type == WORD_FILEOUT_APPEND)
		{
			exec->fileout = open(exec->in->value, O_APPEND | O_CREAT | O_RDWR, 0000644);
			if (exec->fileout == -1)
				return (-1);
			return (0);
		}
	}
	return (1);
}

int	open_files(t_all *p, t_exec *exec)
{
	int	result;
	int	result2;
	
	//Add rules for pipe
	result = open_filein(exec);
	if (result == 2 || result == -1)
	{
		if (result == 2)
		{
			error_executer(exec->in->value, 1);
			result = 1;
		}
		else if (result == -1)
			error_executer(exec->in->value, 3);
		return (result); //correct
	}
	result2 = open_fileout(exec);
	if (result2 == -1)
	{
		error_executer(exec->in->value, 2);
		return (result2); //correct
	}
	if (result == 0)
		dup2(exec->filein, STDIN_FILENO);
	if (result2 == 0)
		dup2(exec->fileout, STDOUT_FILENO);
	return (0);
}

void	close_files(t_exec *exec)
{
	if (exec->filein != 0)
		close(exec->filein);
	if (exec->fileout != 0)
		close(exec->fileout);
}