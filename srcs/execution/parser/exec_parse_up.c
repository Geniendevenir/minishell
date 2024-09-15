/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parse_up.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 10:05:13 by allan             #+#    #+#             */
/*   Updated: 2024/09/15 14:37:43 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*up_to_cmd(t_ast *current)
{
	while (current->parent && (current->parent->type == WORD_CMD
			|| current->parent->type == WORD_OPTION))
		current = current->parent;
	return (current);
}

t_ast	*up_to_parent_operator(t_ast *current)
{
	while (current->parent)
	{
		if (is_operator(current->parent->type, 2) == 1)
			break ;
		current = current->parent;
	}
	return (current);
}

t_ast	*up_and_check_right(t_ast *current, t_ast **prev)
{
	if (current->parent)
	{
		current = current->parent;
		while (current->parent && current->right == *prev)
		{
			*prev = current;
			current = current->parent;
		}
	}
	return (current);
}
