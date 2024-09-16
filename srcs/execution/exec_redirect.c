/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 20:32:13 by allan             #+#    #+#             */
/*   Updated: 2024/09/16 13:42:42 by allan            ###   ########.fr       */
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
	else if (exec->pipe == 2)
	{
		if (exec->in == NULL)
		{
			exec->in = current;
			exec->redirectin = 1;
		}
		if (exec->out == NULL)
		{
			exec->out = current;
			exec->redirectout = 1;
		}
	}
	else if (exec->pipe == 3)
		redirect_pipe_next(current, exec);
}

void	redirect_pipe_next(t_ast *current, t_exec *exec)
{
	if (exec->in == NULL)
	{
		exec->in = current;
		exec->redirectin = 1;
	}
	if (exec->out != NULL)
		exec->redirectout = 1;
}

int	assign_redirect(t_ast *c, t_exec *ex)
{
	while (c && (ex->redirectin == 0 || ex->redirectout == 0))
	{
		if (is_operator(c->type, 2) == 1)
		{
			if (c->type == TOKEN_PIPE)
				redirect_pipe(c, ex);
			else
				assign_redirect_next(ex, 1);
		}
		if ((c->type == 18 || c->type == 25 || c->type == 29)
			&& ex->redirectin == 0)
			ex->in = c;
		else if ((c->type == WORD_FILEOUT || c->type == WORD_FILEOUT_APPEND)
			&& ex->redirectout == 0)
		{
			if (assign_redirect_next(ex, 2) == -1)
				return (-1);
			ex->out = c;
		}
		if (!c->parent)
			break ;
		c = c->parent;
	}
	return (0);
}

int	assign_redirect_next(t_exec *ex, int option)
{
	if (option == 1)
	{
		if (ex->in != NULL)
			ex->redirectin = 1;
		if (ex->out != NULL)
			ex->redirectout = 1;
		return (0);
	}
	else if (option == 2)
	{
		if (ex->out && ex->out->value && access(ex->out->value, F_OK) == 0)
		{
			ex->fileout = open(ex->out->value, O_TRUNC, 0000644);
			if (ex->fileout == -1)
				return (error_executer(NULL, 2), -1);
			close(ex->fileout);
		}
	}
	return (0);
}
