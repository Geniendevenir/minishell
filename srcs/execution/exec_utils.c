/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 18:39:42 by allan             #+#    #+#             */
/*   Updated: 2024/07/11 17:16:01 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_init(t_exec *exec)
{
	exec->pipe = 0;
	exec->redirectin = 0;
	exec->redirectout = 0;
	exec->in = NULL;
	exec->out = NULL;
	exec->command = NULL;
}

void	exec_free(t_exec *exec)
{
	exec->pipe = 0;
	exec->redirectin = 0;
	exec->redirectout = 0;
	exec->in = NULL;
	exec->out = NULL;
	if (exec->command)
		free_array(exec->command);
	exec->command = NULL;
}

