/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 10:19:55 by allan             #+#    #+#             */
/*   Updated: 2024/09/15 17:17:45 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parser_exec_next(t_all *p, t_ast *cur, char **env)
{
	t_ast	*prev;

	cur = up_to_parent_operator(cur);
	prev = cur;
	cur = up_and_check_right(cur, &prev);
	if (cur->right == prev)
		return (0);
	cur = prev;
	if (is_operator(cur->type, 2) == 0 || p->option == 1)
		cur = get_next_operator(p, cur, 1);
	prev = cur;
	if (parse_operator_or(p, &cur, &prev) == 0)
		return (0);
	else if (parse_operator_and(p, &cur, &prev) == 0)
		return (0);
	if ((!cur->parent && p->option != 0) || is_operator(cur->type, 2) == 0)
		return (0);
	else if (p->option == 1)
		cur = get_next_operator(p, cur, 0);
	if (cur->right)
	{
		p->option = 1;
		executer(p, cur->right, env);
	}
	return (0);
}

int	parse_operator_or(t_all *p, t_ast **current, t_ast **prev)
{
	if ((*current)->type == TOKEN_OR && p->exit_status == 0)
	{
		if ((*current)->parent && ((*current)->type == TOKEN_OR
				|| (*prev) == (*current)->right))
		{
			(*current) = get_next_operator(p, (*current), 0);
			if (*prev == *current)
				return (0);
		}
		while ((*current)->parent && ((*current)->type == TOKEN_OR
				|| (*prev) == (*current)->right))
		{
			(*current) = get_next_operator(p, (*current), 0);
			(*prev) = (*prev)->parent;
		}
		if ((*current)->type == TOKEN_OR
			|| is_operator((*current)->type, 2) == 0)
			return (0);
		if ((!(*current)->parent && p->option != 0)
			|| (!(*current)->parent && (*prev) == (*current)->right))
			return (0);
		p->option = 0;
	}
	return (1);
}

int	parse_operator_and(t_all *p, t_ast **current, t_ast **prev)
{
	if ((*current)->type == TOKEN_AND && p->exit_status != 0)
	{
		while ((*current)->type == TOKEN_AND && (*current)->parent)
		{
			(*current) = get_next_operator(p, (*current), 0);
			if (*prev == *current)
				return (0);
		}
		if ((*current)->type == TOKEN_AND)
			return (0);
		if ((!(*current)->parent && p->option != 0))
			return (0);
		p->option = 0;
	}
	return (1);
}
