/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 19:12:43 by allan             #+#    #+#             */
/*   Updated: 2024/09/15 14:06:11 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_command(t_ast *current)
{
	if (current->type == WORD_CMD)
		return (1);
	while (current)
	{
		if (current->type == WORD_CMD)
			return (1);
		if (!current->left)
			break ;
		current = current->left;
	}
	return (0);
}

t_exec	*last_command(t_exec *exec)
{
	t_exec	*node;

	node = exec;
	while (exec)
	{
		if (!exec->next)
			break ;
		exec = exec->next;
	}
	return (exec);
}

t_exec	*pipe_last(t_exec *exec)
{
	while (exec)
	{
		if (!exec->next)
			return (exec);
		exec = exec->next;
	}
	return (exec);
}

int	pipe_addempty(t_all *p, t_exec **exec)
{
	t_exec	*node;

	node = malloc(sizeof(t_exec));
	if (!node)
		return (1);
	exec_init(node);
	pipe_addback(p, exec, node);
	return (0);
}

void	pipe_addback(t_all *p, t_exec **exec, t_exec *new_node)
{
	t_exec	*last_node;

	last_node = *exec;
	if (p->curr_pipe == 0)
	{
		last_node = *exec;
		*exec = new_node;
		free(last_node);
	}
	else
	{
		last_node = pipe_last(*exec);
		last_node->next = new_node;
	}
}
