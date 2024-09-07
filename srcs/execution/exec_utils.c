/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 18:39:42 by allan             #+#    #+#             */
/*   Updated: 2024/09/07 19:34:22 by allan            ###   ########.fr       */
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
		if (p->curr_pipe == p->max_pipe)
			exec->pipe = 1; //left pipe
		else if (p->curr_pipe == 1)
			exec->pipe = 3; //right pipe
		else
			exec->pipe = 2; //middle pipe	
	}
}

void	reset_pipe(t_all *p, int option)
{
	int	i;

	i = 0;
	if (option == 1)
	{
		dup2(p->std_in, STDIN_FILENO);
		close(p->std_in);
		dup2(p->std_out, STDOUT_FILENO);
		close(p->std_out);
		/* if (p->fd)
		{
			while (i < p->max_pipe / 2)
			{
				close(p->fd[i][0]);
				close(p->fd[i][1]);
				i++;
			}
			free(p->fd);
		} */
	}
	else if (p->max_pipe > 0)
	{
		dup2(p->std_out, STDOUT_FILENO);
		if (p->curr_pipe == 3)
			p->curr_pipe = 1;
		else if (p->curr_pipe <= 1)
		{
			write(2, "RESET\n", 6);
			p->max_pipe = 0;
			p->curr_pipe = 0;
			dup2(p->std_in, STDIN_FILENO);
			close(p->std_in);
			close(p->fd[0]);
			dup2(p->std_out, STDOUT_FILENO);
			close(p->std_out);
			close(p->fd[1]);
			/* close(p->fd[0]);
			close(p->fd[1]); */
			/* if (p->fd)
			{
				while (i < p->max_pipe / 2)
				{
					close(p->fd[i][0]);
					close(p->fd[i][1]);
					i++;
				}
				free(p->fd);
			} */
		}
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

