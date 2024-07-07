/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parse_command.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 18:37:43 by allan             #+#    #+#             */
/*   Updated: 2024/07/07 13:12:41 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int get_command(t_ast *current, t_exec *exec)
{
	int	size;
		
	size = command_size(current);
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
	command = malloc(sizeof(char *) * size + 1);
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
