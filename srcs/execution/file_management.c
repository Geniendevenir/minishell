/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_management.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 16:21:55 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/30 15:24:32 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Initialise une structure t_file avec les valeurs par défaut
t_file *init_files(t_ast *current)
{
	t_file	*file;	
	file = malloc(sizeof(t_file));
	if (!file)
	    return NULL;
	file->fd = -1;
	file->type = current->type;
	file->data = ft_strdup(current->value);
	file->next = NULL;
	return file;
}

void	add_file(t_file **files, t_ast *current)
{
	t_file	*new_file;
		
	new_file = init_files(current);
	if (!new_file)
	    return;
	//new_file->fd = open(current->value, O_RDONLY);
	if (new_file->fd == -1)
	{
		free(new_file);
		return;
	}
	new_file->type = current->type;
	new_file->data = ft_strdup(current->value);
	if (!new_file->data)
	{
		//close(new_file->fd);
		free(new_file);
		return;
	}
	new_file->next = *files;
	*files = new_file;
}
