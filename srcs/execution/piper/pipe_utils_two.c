/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils_two.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 22:00:35 by allan             #+#    #+#             */
/*   Updated: 2024/09/15 17:04:13 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipe_allocation(t_all *p, int **pid)
{
	if (p->skip == p->max_pipe)
		p->fd = malloc(sizeof(int [2]) * 1);
	else
		p->fd = malloc(sizeof(int [2]) * (p->max_pipe - p->skip));
	if (!p->fd)
		return (1);
	*pid = malloc(sizeof(int) * ((p->max_pipe - p->skip) + 1));
	if (!(*pid))
	{
		if (p->fd)
			free(p->fd);
		return (1);
	}
	return (0);
}

t_exec	*pipe_skip_empty(t_all *p, t_exec *node)
{
	int	i;

	i = 0;
	p->max_pipe = (p->max_pipe - p->skip);
	while (p->skip > 0)
	{
		node = node->next;
		i = 1;
		p->skip--;
	}
	if (i == 1)
		p->skip = 1;
	return (node);
}

int	pipe_hate_norm(t_all *p, int *i, int option)
{
	if (option == 0)
	{
		if (p->curr_pipe < p->max_pipe)
		{
			if (pipe(p->fd[p->curr_pipe]) == -1)
				return (error_executer(NULL, 10), 1);
		}
	}
	else if (option == 1)
	{
		*i += 1;
		p->curr_pipe++;
	}
	return (0);
}

void	pipe_error(int option)
{
	if (option == 1)
		write(2, "Error: Opening /dev/null\n", 25);
	else if (option == 2)
		write(2, "Error redirecting stdin to /dev/null\n", 37);
	else if (option == 3)
		write(2, "Error Dup2 Redirection Failed\n", 30);
}

/* void	pipe_print(t_exec *exec)
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
} */
