/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:26:52 by allan             #+#    #+#             */
/*   Updated: 2024/09/15 17:22:18 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipe_exec(t_all *p, t_exec **exec, char **env, int *pid)
{
	t_exec	*node;
	int		i;

	i = 0;
	node = *exec;
	if (p->skip == p->max_pipe)
		return (pipe_exec_last(p, node, env, pid));
	node = pipe_skip_empty(p, node);
	while (p->curr_pipe <= p->max_pipe)
	{
		if (pipe_hate_norm(p, &i, 0) == 1)
			return (1);
		pid[i] = fork();
		if (pid[i] == -1)
			return (error_executer(NULL, 5), 1);
		else if (pid[i] == 0)
			pipe_exec_child(p, node, env, 0);
		pipe_hate_norm(p, &i, 1);
		if (!node->next)
			break ;
		node = node->next;
	}
	return (pipe_close_fd(p, 2), wait_childs(p, pid, *exec));
}

void	pipe_exec_child(t_all *p, t_exec *node, char **env, int option)
{
	close(p->std_in);
	close(p->std_out);
	if (open_files(p, node) == 1)
		exit(1);
	if (open_pipe(p, node, option) == 1)
		return (pipe_close_fd(p, 3), exit(1));
	if (*node->command == NULL)
		exit(0);
	if (is_builtin(node->command[0]) == 1 && p->exit_status == 0)
	{
		p->exit_status = exec_builtin(&p, node, node->command);
		exit(p->exit_status);
	}
	else if (p->exit_status == 0)
	{
		p->exit_status = check_cmd(node, p->env);
		if (p->exit_status == 0)
		{
			if (execve(node->path, node->command, env) == -1)
				write(2, "Error: Execve execution failed\n", 31);
		}
		pipe_close_fd(p, 3);
		exit(-1);
	}
}

int	wait_childs(t_all *p, int *pid, t_exec *exec)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (i < p->max_pipe + 1)
	{
		if (pid[i])
		{
			if (waitpid(pid[i], &status, 0) == -1)
				return (error_executer(NULL, 9), 1);
		}
		i++;
		if (exec->next)
			exec = exec->next;
	}
	if (WIFEXITED(status))
		p->exit_status = WEXITSTATUS(status);
	else
		p->exit_status = status;
	return (0);
}

int	pipe_exec_last(t_all *p, t_exec *node, char **env, int *pid)
{
	int	status;

	status = 0;
	node = last_command(node);
	p->max_pipe = 1;
	p->curr_pipe = 1;
	pid[0] = fork();
	if (pid[0] == -1)
		return (error_executer(NULL, 5), 1);
	else if (pid[0] == 0)
		pipe_exec_child(p, node, env, 1);
	if (waitpid(pid[0], &status, 0) == -1)
		return (error_executer(NULL, 9), 1);
	if (WIFEXITED(status))
		p->exit_status = WEXITSTATUS(status);
	else
		p->exit_status = status;
	return (0);
}
