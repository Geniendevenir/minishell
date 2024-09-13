/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:22:25 by allan             #+#    #+#             */
/*   Updated: 2024/09/09 18:53:40 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//printf("test 1\n");

int	pipe_parser(t_all *p, t_ast *current, t_exec **exec, int option)
{
	t_exec	*node;

	node = malloc(sizeof(t_exec));
	/* if (!node)
	{
		add error
	} */
	exec_init(node);
	printf("test 1\n");
	current = up_to_cmd(current);
	if (get_command(current, node) == 1)
	{
		//write(2, "Get Command Error Malloc\n", 25);
		return (1);
	}
	if (node->command[0])
	{
		set_pipe(p, node);
		assign_redirect(current, node);
	}
	printf("test 4\n");
	pipe_addback(p, exec, node);
	if (option == 0)
		pipe_parser_next(p, current, exec);
	return (0);
}

void	pipe_parser_next(t_all *p, t_ast *current, t_exec **exec)
{
	while(p->curr_pipe < p->max_pipe)
	{
		if (!current->parent)
			break ;
		current = current->parent;
		if (current->type == TOKEN_PIPE)
		{
			p->curr_pipe++;
			if (current->right)
				pipe_parser(p, current->right, exec, 1);
		}
	}
}

void	pipe_addback(t_all *p, t_exec **exec, t_exec *new_node)
{
	t_exec	*last_node;

	last_node = *exec;
	if (p->curr_pipe == 0)
	{
		printf("test 5\n");
		last_node = *exec;
		*exec = new_node;
		free(last_node);
	}
	else
	{
		printf("test 6\n");
		last_node = pipe_last(*exec);
		last_node->next = new_node;
	}
}

t_exec *pipe_last(t_exec *exec)
{
	while (exec)
	{
		if (!exec->next)
			return (exec);
		exec = exec->next;
	}
	return (exec);
}