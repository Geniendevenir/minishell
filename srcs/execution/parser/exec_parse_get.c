/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parse_get.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 10:51:53 by allan             #+#    #+#             */
/*   Updated: 2024/09/15 11:16:29 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast *get_next_operator(t_all *p, t_ast *current, t_ast **prev, int option)
{
	if (current->parent)
		current = current->parent;
	while (current->parent)
	{
		if (is_operator(current->type, 2))
			break ;
		current = current->parent;
	}
	if (option == 1)
		p->option = 0;
	return (current);
}

t_ast *get_last_pipe(t_ast *current)
{
	while (current && current->parent && current->type != TOKEN_PIPE)
		current = current->parent;
	while (current && current->parent && current->parent->type == TOKEN_PIPE)
		current = current->parent;
	return (current);
}
