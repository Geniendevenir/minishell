/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 20:32:13 by allan             #+#    #+#             */
/*   Updated: 2024/07/11 17:13:27 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect_pipe(t_ast *current, t_exec *exec)
{
	if (exec->pipe == 1)
	{
		if (exec->out == NULL)
		{
			exec->out = current;
			exec->redirectout = 1;
		}
		if (exec->in != NULL)
		exec->redirectin = 1;
	}
	else if (exec->pipe == 3)
	{
		if (exec->in == NULL)
		{
			exec->in = current;
			exec->redirectin = 1;
		}
		if (exec->out != NULL)
		exec->redirectout = 1;
	}
}

int		assign_redirect(t_ast *current, t_exec *exec)
{
	while (current && (exec->redirectin == 0 || exec->redirectout == 0))
	{
		if (is_operator(current->type, 2) == 1) //En remontant Si je croise un pipe, pipe 1 = gauche/ pipe 2 = millieu / pipe 3 = droite
		{
			if (current->type == TOKEN_PIPE)
				redirect_pipe(current, exec);
			else
			{
				if (exec->in != NULL)
					exec->redirectin = 1;
				if (exec->out != NULL)
					exec->redirectout = 1;
			}
		}
		if ((current->type == WORD_FILEIN || current->type == WORD_LIMITER) && exec->redirectin == 0)
			exec->in = current;
		else if ((current->type == WORD_FILEOUT || current->type == WORD_FILEOUT_APPEND) && exec->redirectout == 0)
			exec->out = current;
		if (!current->parent)
			break;
		current = current->parent;
	}
	if (exec->in)
		printf("in = %s\n", exec->in->value);
	else
		printf("in = NULL\n");
	if (exec->out)
		printf("out = %s\n", exec->out->value);
	else
		printf("out = NULL\n");
	return (0);
}
