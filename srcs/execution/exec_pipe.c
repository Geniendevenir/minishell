/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:16:32 by allan             #+#    #+#             */
/*   Updated: 2024/07/11 17:16:45 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_pipe(t_exec *exec, int set_pipe)
{
	if (set_pipe == 1)
		exec->pipe = 1;
	else if (set_pipe == 2)
		exec->pipe = 2;
	else if (set_pipe == 3)
		exec->pipe = 3;
}
