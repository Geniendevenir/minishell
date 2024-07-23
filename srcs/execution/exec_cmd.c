/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:24:24 by allan             #+#    #+#             */
/*   Updated: 2024/07/22 17:56:12 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_builtin(char **cmd)
{
	int	result;
	
	if (cmd == NULL || cmd[0] == NULL)
		return (0);
	else if (ft_strcmp(cmd[0], "echo") == 0)
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
	/* else if (ft_strcmp(cmd[0], "pwd") == 0)
		result = ft_pwd(cmd + 1); */
	/*
	else if (ft_strcmp(cmd[0], "export") == 0)
		result = ft_export(cmd + 1);
	else if (ft_strcmp(cmd[0], "unset") == 0)
		result = ft_unset(cmd + 1);
	else if (ft_strcmp(cmd[0], "env") == 0 && cmd[1] == NULL)
		result = ft_env(cmd + 1);
	else if (ft_strcmp(cmd[0], "exit") == 0)
		result = ft_exit(cmd + 1, 0); //TROUVER UN MOYEN DE set le CHILD */
	return (result);
}

/* int	exec_cmd(t_exec *exec)
{
	
	
} */