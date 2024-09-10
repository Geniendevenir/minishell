/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piper.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 17:46:20 by allan             #+#    #+#             */
/*   Updated: 2024/09/10 18:03:25 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
1: Simple Pipe
2: Simple Pipe with Blank
3: Multiple Pipe
4: Multiple Pipe with Blank
5: Error Managment
6: Mix Pipe et &&|| Blank or not...
*/

void	pipe_print(t_exec *exec)
{
	while (exec)
	{
		printf("COMMAND = \n");
		if (exec->command)
			print_tab(exec->command);
		if (exec->in)
			printf("in = %s\n", exec->in->value);
		else
			printf("in = NULL\n");
		if (exec->out)
			printf("out = %s\n", exec->out->value);
		else
			printf("out = NULL\n");
		if (!exec->next)
			break ;
		exec = exec->next;
	}
}

int	piper(t_all *p, t_ast *current, char **env)
{
	t_exec *exec;
	int		*pid;
	int		*status;
/* 
	close(p->std_in);
	close(p->std_out); */
	exec = malloc(sizeof(t_exec));
	if (!exec)
		//add error
	exec_init(exec);
	pipe_parser(p, current, &exec, 0);
	pipe_print(exec);
	p->curr_pipe = 0;
	p->fd = malloc(sizeof(int[2]) * p->max_pipe);
	pid = malloc(sizeof(int) * p->max_pipe + 1);
	/* if (!p->fd)
		add error
	*/
	pipe_exec(p, &exec, env, pid);
	free(pid);
	pipe_free(p, exec);
	return (0);
}

int	pipe_allocation(t_all *p)
{
	int		i;

	i = 0;
	printf("p->max_pipe = %d\n", p->max_pipe);
	p->fd = malloc(sizeof(int[2]) * p->max_pipe);
	if (!p->fd)
		return (1);
	/* while (i < p->max_pipe)
	{
		if (pipe(p->fd[i]) == -1)
		{
			//ADD close pipe
			free(p->fd);
			return (1);
		}
		i++;
	} */
	return (0);
}
