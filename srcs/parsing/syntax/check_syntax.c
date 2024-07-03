/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 16:42:50 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/03 18:40:30 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_operator(t_token *c)
{
	if (c->type == 4 || c->type == 5 || c->type == 6 || c->type == 7
		|| c->type == 8 || c->type == 9 || c->type == 10)
			return (1);
	return (0);
}

int		double_operator(t_token *current)
{
	if (current->next)
	{
		if (is_operator(current) && is_operator(current->next))
		{
			error_syntax(current->next, 1);
			return (1);
		}
	}
	return (0);
}

bool 	check_syntax(t_token *current)
{
	if (!current)
		return (0);
	int			skip;
	t_syntax	syntax;
	
	skip = 0;
	syntax.openpar = 0;
	syntax.operator = 0;
	while (current)
	{
		if (double_operator(current) == 1)
				return (1);
		if (skip > 0)
			skip--;
		if (skip == 0 && (current->type == TOKEN_OPENPAR || current->type == TOKEN_CLOSEPAR))
		{
			if (check_parenthesis(current, syntax, &skip) == 1)
				return (1);
		}
		current = current->next;
	}
	return (0);
}
