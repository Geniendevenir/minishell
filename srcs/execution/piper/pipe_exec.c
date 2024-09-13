/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:26:52 by allan             #+#    #+#             */
/*   Updated: 2024/09/13 13:56:41 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//echo test 
//echo test | $skjdfsij | cat
//max_pipe = 2, skip = 2
//fd malloc * 1 && pid malloc * 1
//max_pipe = 1, curr_pipe == 1
//echo test | $skjdfsij | cat | cat
//max_pipe = 3, skip = 2
//fd malloc * 1 && pid malloc * 2
//max_pipe = 1, curr_pipe == 0

int	pipe_exec(t_all *p, t_exec **exec, char **env, int *pid)
{
	t_exec	*node;
	int		i;
	int		status;

	i = 0;
	status = 0;
	node = *exec;
	printf("p->max_pipe = %d\n", p->max_pipe);
	if (p->skip == p->max_pipe)
		return (pipe_exec_last(p, node, env, pid));
		//p->max_pipe = 1;
	else
	p->max_pipe = (p->max_pipe - p->skip);
	printf("p->skip = %d\n", p->skip);
	printf("p->max_pipe = %d\n", p->max_pipe);
	while (p->skip > 0)
	{
		node = node->next;
		i = 1;
		p->skip--;
	}
	if (i == 1)
		p->skip = 1;
	i = 0;
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
			pipe_exec_child(p, node, env, 0);
		i++;
		p->curr_pipe++;
		if (!node->next)
			break ;
		node = node->next;
	}
	printf("TEEEST\n");
	pipe_close_fd(p, 2);
	wait_childs(p, pid, *exec);
	return (0);
}

int	pipe_exec_child(t_all *p, t_exec *node, char **env, int option)
{
	printf("CHILD:\n");
	close(p->std_in);
	close(p->std_out);
	p->exit_status = open_files(node);
	if (p->exit_status == 1)
		exit(p->exit_status);
	p->exit_status = open_pipe(p, node, option);
	if (p->exit_status == 1)
		exit(p->exit_status);
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
			printf("cmd = %s\n", node->command[0]);
			printf("path = %s\n", node->path);
			if (execve(node->path, node->command, env) == -1)
				write(2, "Error: Execve execution failed\n", 31);
		}
		exit(-1);
	}
	return (0);
}

int	wait_childs(t_all *p, int *pid, t_exec *exec)
{
	int	status;
	int	i;

	i = 0;
	status = 0;
	printf("wait nbr = %d\n", p->max_pipe + 1);
	while (i < p->max_pipe + 1)
	{
		if (pid[i])
		{
			printf("wait: %d\n", pid[i]);
			if (waitpid(pid[i], &status, 0) == -1)
			{
				//add error
				error_executer(NULL, 9);
				return (-1);
			}
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

int	pipe_exec_last(t_all *p, t_exec *node, char **env, int *pid)
{
	int	status;
	
	printf("EXEC LAST\n");
	node = last_command(node);
	p->max_pipe = 1;
	p->curr_pipe = 1;
	/* if (pipe(p->fd[0]) == -1)
	{
		//ADD close pipe
		free(p->fd);
		return (1);
	} */
	pid[0] = fork();
	if (pid[0] == -1)
	{
		//add error
		return (-1); //does not set $?
	}
	else if (pid[0] == 0)
		pipe_exec_child(p, node, env, 1);
	//pipe_close_fd(p, 2);
	if (waitpid(pid[0], &status, 0) == -1)
	{
		//add error
		error_executer(NULL, 9);
		return (-1);
	}
	if (WIFEXITED(status))
		p->exit_status = WEXITSTATUS(status);
	else
		p->exit_status = status;
	return (0);
}
