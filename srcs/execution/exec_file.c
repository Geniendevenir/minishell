/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:11:24 by allan             #+#    #+#             */
/*   Updated: 2024/09/07 19:11:37 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
1 - open redirection files
2 - si au premier pipe creer le pipe
3 - fork child -> faire les redirections -> executer
*/

int	open_files(t_all *p, t_exec *exec)
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
		dup2(exec->filein, STDIN_FILENO);
	if (result2 == 0)
		dup2(exec->fileout, STDOUT_FILENO);
	//open_pipe(p, exec);
	return (open_pipe(p, exec));
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


int		open_pipe(t_all *p, t_exec *exec)
{
	if (p->max_pipe > 0 && p->curr_pipe >= 1)
	{
		if (p->curr_pipe == p->max_pipe)
		{
			if (pipe(p->fd) == -1)
			{
				error_executer(exec->in->value, 10);
				return (1);
			}
		}
		if (exec->in && exec->in->type == TOKEN_PIPE)
		{
			dup2(p->fd[0], STDIN_FILENO);
			close(p->fd[0]);
		}
		if (exec->out && exec->out->type == TOKEN_PIPE)
		{
			dup2(p->fd[1], STDOUT_FILENO);
			close(p->fd[1]);
		}
		else if (p->curr_pipe == 1 && !exec->out)
		{
			write(2, "DERNIER REDIRECT\n", 17);
			//dup2(p->std_in, STDIN_FILENO);
			dup2(p->std_out, STDOUT_FILENO);
			close(p->std_out);
			//close(p->std_out);
			//close(p->fd[0]);
			//close(p->fd[1]);
		}
	}
	return (0);
}

/* int		open_pipe(t_all *p, t_exec *exec)
{
	int	i;
	int	current;
	
	if (p->max_pipe > 0 && p->curr_pipe > 1)
	{
		current = p->max_pipe - p->curr_pipe;
		if (p->curr_pipe == p->max_pipe) // premiere commande
		{
			printf("p->max_pipe / 2 = %d\n", p->max_pipe / 2);
			p->fd = malloc(sizeof(int[2]) * p->max_pipe / 2);
			if (!p->fd)
			{
				error_executer(exec->in->value, 4);
				return (1);
			}
			i = 0;
			while (i < p->max_pipe / 2)
			{
				if (pipe(p->fd[i]) == -1)
				{
					error_executer(exec->in->value, 10);
					return (1);
				}
				i++;
			}
		}
		printf("current = %d\n", current);
		if (exec->in && exec->in->type == TOKEN_PIPE)
			dup2(p->fd[current - 1][0], STDIN_FILENO);
		if (exec->out && exec->out->type == TOKEN_PIPE)
			dup2(p->fd[current][1], STDOUT_FILENO);
		else if (p->curr_pipe == 1 && !exec->out)
		{
			if (dup2(p->std_out, STDOUT_FILENO) < 0)
				return (1);
		}
	}
	return (0);
} */

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