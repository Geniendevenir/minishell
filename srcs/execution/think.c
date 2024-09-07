/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   think.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 20:47:23 by allan             #+#    #+#             */
/*   Updated: 2024/09/02 16:24:31 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



/* int		executer(t_all *p, bool option) 
{
	
	Left
	Up -> Cmd
	execute
	While (current->parent && is_operator(current->type, 2))
	{
		if (operator->right)
		{
			reset_exec;
			executer(p {p->right});
			return (0);
		}
		current = current->parent;
	}
	
} */

/*

int		redirect_pipe(t_all *p, t_exec *exec)
{
	if (p->max_pipe > 0 && p->curr_pipe > 1)
	{
		if (p->curr_pipe % 2 == 0)
		{
			if (pipe(p->fd1) == -1)
			{
				ERROR_PIPE
			}
			if (exec->in == PIPE)
				redirect pipe
			if (exec->out == PIPE)
				redirect pipe
		}
		else
		{
			if (pipe(p->fd2) == -1)
			{
				ERROR_PIPE
			}
			if (exec->in == PIPE)
				redirect pipe
			if (exec->out == PIPE)
				redirect pipe
		}
	}
	return (0);
}

*/