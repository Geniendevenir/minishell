/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:47:38 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/10 18:36:45 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//RECHECK TOUS LES RETURNS / EXIT STATUS
int		check_cmd(t_exec	*exec, t_env *env)
{
	char	*path;
	char	*temp;
	int		error;

	error = 0;
	temp = NULL;
	if (!exec->command || !exec->command[0])
		return (1);
	//test builtin
	//printf("exec->command[0] = %s\n", exec->command[0]);
	if (access(exec->command[0], X_OK) == -1) //check if not absolute path
	{
		if (check_builtin(exec->command[0]))
			return (0);
		path = get_path(exec->command[0], env, &error);
		if (error != 0)
		{
			printf("ERROR: get_path error\n");
			if (path)
				free (path);
			return (1); //malloc error
		}
		if (!path)
		{
			write(1, exec->command[0], ft_strlen(exec->command[0]));
			write(1, ": command not found\n", 20);
			return (127);
		}
		temp = exec->command[0];
		free(temp); // Ca sert a qqchose ?
		exec->command[0] = path;
	}
	return (0);
}
int	open_filein(t_exec **exec)
{
	if ((*exec)->in)
	{
		(*exec)->filein = open((*exec)->in->value, O_RDONLY);
		if ((*exec)->filein == -1)
			return (-1);
		return (0);
	}
	return (-1);
}

int	open_fileout(t_exec **exec)
{
	if ((*exec)->out)
	{
		if ((*exec)->out->type == WORD_FILEOUT)
		{
			(*exec)->fileout = open((*exec)->in->value, O_TRUNC | O_CREAT | O_RDWR, 0000644);
			if ((*exec)->fileout == -1)
				return (-1);
			return (0);
			
		}
		else if((*exec)->out->type == WORD_FILEOUT_APPEND)
		{
			(*exec)->fileout = open((*exec)->in->value, O_APPEND | O_CREAT | O_RDWR, 0000644);
			if ((*exec)->fileout == -1)
				return (-1);
			return (0);
		}
	}
	return (-1);
}

int		exec_command(t_all *p, t_exec *exec)
{
	if (open_filein(&exec) == -1)
	{
		if (dup2())
	}
	else
		dup();
	if (open_fileout(&exec) == -1)
	{
		if (dup2())
	}
	else
		dup();
}