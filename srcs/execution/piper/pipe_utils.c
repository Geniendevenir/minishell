/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 19:12:43 by allan             #+#    #+#             */
/*   Updated: 2024/09/13 10:58:55 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_command(t_ast *current)
{
	if (current->type == WORD_CMD)
		return (1);
	while (current)
	{
		if (current->type == WORD_CMD)
			return (1);
		if (!current->left)
			break ;
		current = current->left;
	}
	return (0);
}

t_exec *	last_command(t_exec *exec)
{
	t_exec	*node;

	node = exec;
	while (exec) //Se placer a la derniere commande
	{
		if (!exec->next)
			break ;
		exec = exec->next;
	}
	return (exec);
}
