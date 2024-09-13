/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piper.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 17:46:20 by allan             #+#    #+#             */
/*   Updated: 2024/09/13 11:14:35 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
1: Simple Pipe OK
2: Simple Pipe with Blank OK
3: Multiple Pipe OK
4: Multiple Pipe with Blank OK
7: Multiple Pipe with Redirection
5: Error Managment + Exit Status
6: Mix Pipe et &&|| Blank or not...
8: NORME
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
//$djsjkf | echo test | cat
//$kjsdnfj $sdkjhfsjf | $sdkfsfii | echo test | cat
//$szjkbfj | echo test
//max_pipe = 1;
//skip = 1;
int	piper(t_all *p, t_ast *current, char **env)
{
	t_exec *exec;
	int		*pid;
	
	printf("p->max_pipe = %d\n", p->max_pipe);
/* 
	close(p->std_in);
	close(p->std_out); */
	pid = NULL;
	exec = malloc(sizeof(t_exec));
	if (!exec)
	{
		error_executer(NULL, 4);
		return (1);
	}
	exec_init(exec);
	pipe_parser(p, current, &exec, 0);
	if (pipe_analyser(p, exec) == 1)
	{
		write(2, "ERROR: Pipe last command is Empty\n", 34);
		pipe_free(p, exec, pid);
		return (0);
	}
	pipe_print(exec);
	p->curr_pipe = 0;
	if (p->skip == p->max_pipe)
		p->fd = malloc(sizeof(int[2]) * 1);
	else
		p->fd = malloc(sizeof(int[2]) * (p->max_pipe - p->skip));
	if (!p->fd)
	{
		pipe_free(p, exec, pid);
		error_executer(NULL, 4);
		return (1);
	}
	printf("size pid = %d\n", (p->max_pipe - p->skip) + 1);
	pid = malloc(sizeof(int) * ((p->max_pipe - p->skip) + 1));
	if (!pid)
	{
		pipe_free(p, exec, pid);
		error_executer(NULL, 4);
		return (1);
	}
	pipe_exec(p, &exec, env, pid);
	pipe_free(p, exec, pid);
	return (0);
}

int	pipe_analyser(t_all *p, t_exec *exec)
{
	t_exec	*node;
	int	i;

	node = exec;
	i = 1;
	p->skip = 0;
	while (node) //how much cmd are blank
	{
		printf("turn\n");
		if (node->command == NULL) //(!node->in || (node->in && is_operator(node->in->type, 3) == 0))
			p->skip = i;
		if (!node->next)
			break ;
		i++;
		node = node->next;
	}
	printf("skip = %d\n", p->skip);
	if (p->skip == (p->max_pipe + 1)) //all cmd are blank or the last cmd is
		return (1);
	node = exec;
	return (0);	
}

//echo test | $djskjdf => all cmd are blank
//$sdhfis | echo test
//echo test | $sdkjfjs | echo ok
//echo test > output.txt | $sdkjfjs | echo ok
// 2 -> 0

