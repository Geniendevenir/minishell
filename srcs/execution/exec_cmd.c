/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:24:24 by allan             #+#    #+#             */
/*   Updated: 2024/08/15 20:11:06 by Matprod          ###   ########.fr       */
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


int	path_comp_builtins(char **cmd)
{
	if (cmd == NULL || cmd[0] == NULL)
		return (0);
	else if (ft_strcmp(cmd[0], "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd[0], "cd") == 0)
		return (2);
	else if (ft_strcmp(cmd[0], "pwd") == 0)
		return (3);
	else if (ft_strcmp(cmd[0], "export") == 0)
		return (4);
	else if (ft_strcmp(cmd[0], "unset") == 0)
		return (5);
	else if (ft_strcmp(cmd[0], "env") == 0 && cmd[1] == NULL)
		return (6);
	else if (ft_strcmp(cmd[0], "exit") == 0)
		return (7);
	return (0);
}

int	exec_builtin(t_all **p, char **cmd, t_env **env_list)
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
		result = ft_cd(cmd[1]);
	}
	else if (ft_strcmp(cmd[0], "pwd") == 0)
		result = ft_pwd(cmd[1]);
	else if (ft_strcmp(cmd[0], "export") == 0)
		result = ft_export(cmd + 1, env_list);
	else if (ft_strcmp(cmd[0], "unset") == 0)
		result = ft_unset(*env_list, cmd + 1);
	else if (ft_strcmp(cmd[0], "env") == 0 && cmd[1] == NULL)
		result = ft_env(*env_list);
	else if (ft_strcmp(cmd[0], "exit") == 0)
		ft_exit(p, cmd + 1); //TROUVER UN MOYEN DE set le CHILD
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
		return (-1);
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
