/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:26:52 by allan             #+#    #+#             */
/*   Updated: 2024/09/10 18:02:03 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipe_exec(t_all *p, t_exec **exec, char **env, int *pid)
{
	t_exec	*node;
	int		i;
	int		status;

	i = 0;
	status = 0;
	node = *exec;
	while (p->curr_pipe <= p->max_pipe)
	{
		if (p->curr_pipe < p->max_pipe)
		{
			printf("TEST\n");
			if (pipe(p->fd[p->curr_pipe]) == -1)
			{
				//ADD close pipe
				free(p->fd);
				return (1);
			}
		}
		pid[i] = fork();
		if (pid[i] == -1)
		{
			//add error
			return (-1); //does not set $?
		}
		else if (pid[i] == 0)
		{
			pipe_exec_child(p, node, env);
		}
		//pipe_close_fd(p);
		i++;
		p->curr_pipe++;
		if (!node->next)
			break ;
		node = node->next;
	}
	pipe_close_fd(p);
	wait_childs(p, pid);
	//pipe_exec_parent(p, pid, status);
	return (0);
}

int	pipe_exec_child(t_all *p, t_exec *node, char **env)
{
	printf("CHILD:\n");
	/* close(p->std_in);
	close(p->std_out); */
	p->exit_status = open_files(node);
	if (p->exit_status == 1)
		exit(p->exit_status);
	p->exit_status = open_pipe(p, node);
	if (p->exit_status == 1)
		exit(p->exit_status);
	
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
			printf("cmd = %s\n", node->command[0]);
			printf("path = %s\n", node->path);
			if (execve(node->path, node->command, env) == -1)
				write(2, "Error: Execve execution failed\n", 31);
		}
		exit(-1);
	}
	return (0);
}

int	wait_childs(t_all *p, int *pid)
{
	int	status;
	int	i;

	i = 0;
	status = 0;
	while (i < p->max_pipe +1)
	{
		if (waitpid(pid[i], &status, 0) == -1)
		{
			//add error
			error_executer(NULL, 9);
			return (-1);
		}
		i++;
	}
	if (WIFEXITED(status))
		p->exit_status = WEXITSTATUS(status);
	else
		p->exit_status = status;
	return (0);
}

int	pipe_exec_parent(t_all *p, int pid, int status)
{
	if (p->curr_pipe == 0)
		close(p->fd[p->curr_pipe - 1][1]);
	else if (p->curr_pipe > 0 && p->curr_pipe < p->max_pipe)
	{
		close(p->fd[p->curr_pipe - 1][0]);
		close(p->fd[p->curr_pipe][1]);
	}
	else if (p->curr_pipe == p->max_pipe)
	{
		close(p->fd[p->curr_pipe - 1][0]);
	}
	if (waitpid(pid, &status, 0) == -1)
	{
		//add error
		error_executer(NULL, 9);
		return (-1);
	}
	/* printf("status = %d\n", status);
	printf("WIFEXITED(status) = %d\n", WIFEXITED(status)); */
	if (WIFEXITED(status))
		p->exit_status = WEXITSTATUS(status);
	else
		p->exit_status = status;
	return (0);
}
