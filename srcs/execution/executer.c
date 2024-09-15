/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 21:23:21 by allan             #+#    #+#             */
/*   Updated: 2024/09/15 16:43:45 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	executer(t_all *p, t_ast *current, char **env)
{
	t_exec	exec;

	exec_init(&exec);
	p->error = 0;
	current = down_left_expand(p, current, 0);
	if (p->error == 1)
		return (exec_free(&exec), 1);
	if (p->max_pipe > 0)
	{
		if (piper(p, current, env) == 1)
			return (exec_free(&exec), 1);
		current = get_last_pipe(current);
		if (!current->parent)
			return (exec_free(&exec), 0);
	}
	else
	{
		if (execute_command(p, &current, &exec, env) == 1)
			return (1);
	}
	if (close_files(&exec, p) == 1)
		return (exec_free(&exec), 1);
	exec_free(&exec);
	parser_exec_next(p, current, env);
	return (0);
}

int	execute_command(t_all *p, t_ast **current, t_exec *exec, char **env)
{
	*current = up_to_cmd(*current);
	if ((*current) && (*current)->value && (*current)->type == WORD_CMD)
	{
		if (get_command(*current, exec) == 1)
			return (error_executer(NULL, 4), exec_free(exec), 1);
		if (exec->command[0])
		{
			assign_redirect((*current), exec);
			if (open_files(p, exec) == 1)
				return (close_files(exec, p), exec_free(exec), 1);
			if (is_builtin(exec->command[0]) == 1 && p->exit_status == 0)
				p->exit_status = exec_builtin(&p, exec, exec->command);
			else if (p->exit_status == 0)
			{
				p->exit_status = check_cmd(exec, p->env);
				if (p->exit_status == 0)
				{
					p->exit_status = exec_cmd(p, exec, env);
					if (p->exit_status < 0)
						return (close_files(exec, p), exec_free(exec), 1);
				}
			}
		}
	}
	return (0);
}
