/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:24:24 by allan             #+#    #+#             */
/*   Updated: 2024/09/14 22:45:07 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int	exec_builtin(t_all **p, t_exec *exec, char **cmd)
{
	int	result;
	
	result = 0;
	if (ft_strcmp(cmd[0], "echo") == 0)
		result = ft_echo(cmd + 1);
	else if (ft_strcmp(cmd[0], "cd") == 0)
	{
		if (array_size(cmd) > 2)
		{
			error_executer(NULL, 6);
			return (1);	
		}
		result = ft_cd((*p)->env ,cmd);
	}
	else if (ft_strcmp(cmd[0], "pwd") == 0)
		result = ft_pwd(cmd[1]);
	else if (ft_strcmp(cmd[0], "export") == 0)
		result = ft_export((*p)->env, cmd);
	else if (ft_strcmp(cmd[0], "unset") == 0)
		result = ft_unset((*p)->env, cmd + 1);
	else if (ft_strcmp(cmd[0], "env") == 0 && cmd[1] == NULL)
		result = ft_env((*p)->env);
	else if (ft_strcmp(cmd[0], "exit") == 0)
		result = ft_exit(p, exec, cmd);
	return (result);
}

int	exec_cmd(t_exec *exec, int *exit_status, char **env)
{
	int	pid;
	int	status;

	pid = fork();
	if (pid == -1)
	{
		error_executer(NULL, 8);
		return (-1); //does not set $?
	}
	else if (pid == 0)
	{
		if (execve(exec->path, exec->command, env) == -1)
			write(2, "Error: Execve execution failed\n", 31);
		exit(-1);
	}
	else
	{
		if (waitpid(pid, &status, 0) == -1)
		{
			error_executer(NULL, 9);
			return (-1);
		}
		/* printf("status = %d\n", status);
		printf("WIFEXITED(status) = %d\n", WIFEXITED(status)); */
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		return (status);
	}
	return (0);
}
