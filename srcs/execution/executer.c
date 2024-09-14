/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 21:23:21 by allan             #+#    #+#             */
/*   Updated: 2024/09/14 23:55:22 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/*
	TO DO

	1 - Link Pipe et le reste de l'exec
	2 - Check Error Management + Exit Status (Exec et Pipes)
	3 - Norme

	TO CHECK: add close stdin && out dans exec_cmd
 */


//write(2, "test\n");

int		executer(t_all *p, t_ast *current, char **env)
{
	t_exec	exec;
	t_ast	*prev;

	write(2, "TURN\n", 5);
	exec_init(&exec);
	current = left_expand(p, current, 0);
	if (p->error == 1)
		return (exec_free(&exec), 1);
	if (p->max_pipe > 0)
	{
		if (piper(p, current, env) == 1)
			return (exec_free(&exec), 1);
		current = get_last_pipe(current);
		if (!current->parent)
		{
			printf("IIIICIIIII\n");
			return (exec_free(&exec), 0);
		}
	}
	else
	{
		current = up_to_cmd(current);
		if (current && current->value && current->type == WORD_CMD)
		{
			if (get_command(current, &exec) == 1)
			{
				write(2, "Get Command Error Malloc\n", 25);
				exec_free(&exec);
				return (1);
			}
			if (exec.command[0])
			{
				print_tab(exec.command);
				assign_redirect(current, &exec);
				p->exit_status = open_files(&exec);
				if (p->exit_status == 1)
				{
					close_files(&exec, p);
					exec_free(&exec);
					return (1);
				}
				write(2, "RESULT:\n", 8);
				if (is_builtin(exec.command[0]) == 1 && p->exit_status == 0)
					p->exit_status = exec_builtin(&p, &exec, exec.command);
				else if (p->exit_status == 0)
				{
					p->exit_status = check_cmd(&exec, p->env);
					if (p->exit_status == 0)
					{
						print_tab(exec.command);
						p->exit_status = exec_cmd(&exec, &p->exit_status, env);
						if (p->exit_status < 0)
						{
							write(2, "End child\n", 10);
							close_files(&exec, p);
							exec_free(&exec);
							return (1);
						}
					}
				}
					//if (p->exit_status == 1) stop
			}
		}
	}
	close_files(&exec, p);
	exec_free(&exec);
	while (current->parent)
	{
		if (is_operator(current->parent->type, 2) == 1)
			break ;
		current = current->parent;
	}
	prev = current;
	if (current->parent)
	{
		current = current->parent;
		while (current->parent && current->right == prev)
		{
			prev = current;
			current = current->parent;
		}
	}
	if (current->right == prev)
		return (0);
	current = prev;
	if (is_operator(current->type, 2) == 0 || p->option == 1)
	{
		current = get_next_operator(p, current, &prev);
		p->option = 0;
	}
	prev = current;
	if (current->type == TOKEN_OR && p->exit_status == 0)
	{
		if (current->parent && (current->type == TOKEN_OR || prev == current->right))
		{
			current = get_next_operator(p, current, &prev);
			if (prev == current)
				return (0);
		}
		while (current->parent && (current->type == TOKEN_OR || prev == current->right))
		{
			current = get_next_operator(p, current, &prev);
			prev = prev->parent;
		}
		if (current->type == TOKEN_OR || is_operator(current->type, 2) == 0)
			return (0);
		if ((!current->parent && p->option != 0) || (!current->parent && prev == current->right))
			return (0);
		p->option = 0;
	}
	else if (current->type == TOKEN_AND && p->exit_status != 0)
	{
		while (current->type == TOKEN_AND && current->parent)
		{
			current = get_next_operator(p, current, &prev);
			if (prev == current)
				return (0);
		}
		if (current->type == TOKEN_AND)
			return (0);
		if ((!current->parent && p->option != 0))
			return (0);
		p->option = 0;
	}
	if ((!current->parent && p->option != 0) || is_operator(current->type, 2) == 0)
		return (0);
	else if (p->option == 1)
		current = get_next_operator(p, current, &prev);
	if (current->right)
	{
		p->option = 1;
		write(2, "\n\n", 2);
		executer(p, current->right, env);
	}
	return (0);
}
