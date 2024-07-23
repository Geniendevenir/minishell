/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_check_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 18:37:43 by allan             #+#    #+#             */
/*   Updated: 2024/07/21 13:03:45 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int get_command(t_ast *current, t_exec *exec)
{
	int	size;
		
	size = command_size(current);
	printf("cmd size = %d\n", size);
	exec->command = parse_command(current, size);
	if (!exec->command)
		return (1);
	return (0);
}

int	command_size(t_ast *current)
{
	int size;

	size = 1;
	while (current->left && (current->type == WORD_CMD || current->type == WORD_BUILTIN || current->type == WORD_OPTION))
	{
		if (!current->left->value)
			break ;
		size++;
		current = current->left;
	}
	return (size);
}

char **parse_command(t_ast *current, int size)
{
	char **command;
	int i;

	i = 0;
	command = malloc(sizeof(char *) * (size + 1));
	if (!command)
		return (NULL);
	while (size > 0)
	{
		if (!current->value)
			return (NULL);
		command[i] = ft_strdup(current->value); //pas besoin de free current->value ici
		if (!command[i])
		{
			free_array(command);
			return (NULL);
		}
		i++;
		size--;
		current = current->left;
	}
	command[i] = 0;
	return (command);
}

int		check_cmd(t_exec *exec, t_env *env)
{
	char	*path;
	char	*temp;
	int		error;

	error = 0;
	temp = NULL;
	if (!exec->command || !exec->command[0])
		return (0);
	if (access(exec->command[0], X_OK) == -1) //check if not absolute path
	{
		if (check_builtin(exec->command[0]) == 1)
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
