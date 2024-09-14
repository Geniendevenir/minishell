/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 09:39:16 by allan             #+#    #+#             */
/*   Updated: 2024/09/14 21:53:39 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast *left_expand(t_all *p, t_ast *current, int option)
{
	p->error = 0;
	while (current) //Down->Left : A CHAQUE DESCENTE LEFT OR RIHT EXPAND LES ENV POUR LES WORD ET LES HEREDOC DONT LE TYPE N'EST PAS SQ_LIMITER
	{
		if (current->type == TOKEN_PIPE)
		{
			if (option == 0)
				p->max_pipe += 1;
			if (current->right)
				left_expand(p, current->right, 1);
		}
		if (is_operator(current->type, 1))
			p->option = 0;
		if (current->state == STATE_WORD && current->type == WORD_LIMITER)
			expand_heredoc(p);
		if (current->state == STATE_WORD && current->type != WORD_SQLIMITER && current->type != WORD_LIMITER)
		{
			/* write(2, "current = %s\n", current->value);
			write(2, "current size = %ld\n", ft_strlen(current->value)); */
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

t_ast	*up_to_cmd(t_ast *current)
{
	while (current->parent && (current->parent->type == WORD_CMD
		|| current->parent->type == WORD_OPTION))
		current = current->parent;
	return (current);
}

t_ast *get_next_operator(t_all *p, t_ast *current, t_ast **prev)
{
	if (current->parent)
		current = current->parent;
	while (current->parent)
	{
		if (is_operator(current->type, 2))
			break ;
		current = current->parent;
	}
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