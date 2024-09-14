/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:22:25 by allan             #+#    #+#             */
/*   Updated: 2024/09/14 23:41:36 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//OK

int	pipe_parser(t_all *p, t_ast *current, t_exec **exec, int option)
{
	t_exec	*node;

	node = malloc(sizeof(t_exec));
	if (!node)
		return (1);
	exec_init(node);
	current = down_left(current);
	if (current->state == STATE_WORD && (current->type == WORD_CMD || current->type == WORD_OPTION)) //cmd bottom left blank
	{
		current = up_to_cmd(current);
		if (get_command(current, node) == 1)
			return (1);
	}
	set_pipe(p, node);
	assign_redirect(current, node);
	pipe_addback(p, exec, node);
	if (option == 0)
	{
		if (pipe_parser_next(p, current, exec) == 1)
			return (1);
	}
	return (0);
}

int		pipe_parser_next(t_all *p, t_ast *current, t_exec **exec)
{
	if (current->type == TOKEN_PIPE)
	{
		p->curr_pipe++;
		if (parser_add_pipe(p, current, exec) == 1)
			return (1);
	}
	while(p->curr_pipe < p->max_pipe)
	{
		if (!current->parent)
			break ;
		current = current->parent;
		if (current->type == TOKEN_PIPE)
		{
			p->curr_pipe++;
			if (parser_add_pipe(p, current, exec) == 1)
				return (1);
		}
	}
	return (0);
}

int		parser_add_pipe(t_all *p, t_ast *current, t_exec **exec)
{
		if (current->right && is_command(current->right) == 1)
		{
			if (pipe_parser(p, current->right, exec, 1) == 1)
				return (1);
		}
		else
		{
			if (pipe_addempty(p, exec) == 1)
				return (1);
		}
		return (0);
}

int	pipe_analyser(t_all *p, t_exec *exec)
{
	t_exec	*node;
	int	i;

	node = exec;
	i = 1;
	p->skip = 0;
	while (node) //how much cmd are empty
	{
		if (node->command == NULL) //(!node->in || (node->in && is_operator(node->in->type, 3) == 0))
			p->skip = i;
		if (!node->next)
			break ;
		i++;
		node = node->next;
	}
	if (p->skip == (p->max_pipe + 1)) //all cmd are empty or the last cmd is
		return (1);
	node = exec;
	return (0);	
}
