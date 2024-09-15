/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parse_down.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 09:39:16 by allan             #+#    #+#             */
/*   Updated: 2024/09/15 11:10:40 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast *down_left_expand(t_all *p, t_ast *current, int option)
{
	while (current)
	{
		expand_pipe(p, current, option);
		if (is_operator(current->type, 1))
			p->option = 0;
		if (current->state == STATE_WORD && current->type == WORD_LIMITER)
			expand_heredoc(p);
		if (current->state == STATE_WORD && current->type != WORD_SQLIMITER && current->type != WORD_LIMITER)
		{
			p->error = split_word(p, &current);
			if (!current || !current->value)
				return (current);
			else if (p->error != 0 && p->error != -1)
				return (current); //CHANGE RETURN AS IT ALSO TAKE INTO ACCOUNT EMPTY PROMTP AFTER EXPANDER
			else if (is_operator(current->type, 2) == 1)
				return (current);
		}
		if (p->error != -1 || current->type == TOKEN_PIPE)
		{
			if (!current->left)
				return (current);
			current = current->left;
		}
	}
	return (current);
}

void	expand_pipe(t_all *p, t_ast *current, int option)
{
	if (current->type == TOKEN_PIPE)
	{
		if (option == 0)
			p->max_pipe += 1;
		if (current->right)
			down_left_expand(p, current->right, 1);
	}
}

t_ast *down_left(t_ast *current)
{
	while (current)
	{
		if (!current->left)
			break ;
		current = current->left;
	}
	return (current);
}
