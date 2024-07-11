/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 16:42:50 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/11 16:19:12 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	()
	()()
	& | OK
	
	enum s_type{
	NOT_DEFINE,
	TOKEN_WORD,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_PIPE,
	TOKEN_REDIRECTIN,
	TOKEN_REDIRECTOUT,
	TOKEN_HEREDOC,
	TOKEN_APPENDOUT,
	TOKEN_LIMITER,
	TOKEN_OPENPAR,
	TOKEN_CLOSEPAR,
};

*/

bool	is_operator(enum s_type type, int option)
{
	if (option == 1)
	{
		if (type == TOKEN_AND || type == TOKEN_OR)
			return (1);
	}
	if (option == 2)
	{
		if (type == TOKEN_AND || type == TOKEN_OR
			|| type == TOKEN_PIPE)
			return (1);
	}
	if (option == 3)
	{
		if (type == TOKEN_REDIRECTIN || type == TOKEN_REDIRECTOUT
			|| type == TOKEN_APPENDOUT || type == TOKEN_HEREDOC)
			return (1);
	}
	if (option == 4)
	{
		if (type == TOKEN_OPENPAR || type == TOKEN_CLOSEPAR)
			return (1);
	}
	return (0);
}
/* REGLER PROBLEME:
1
bash: syntax error near unexpected token '&&'
Minishell> test (&&


*/

int	double_operator(t_token *c)
{
	if (!c->next)
	{
		if (if_last_token_is_opearator(c))
			return (1);
	}
	else if (c->next)
	{
		if (if_is_a_redirect(c))
		{
			if (triple_if_in_double_operator(c))
				return (1);
		}
		if (condition_of_multiple_is_ope(c))
		{
			error_syntax(c->next, 1);
			return (1);
		}
		if (c->next->next)
		{
			if (redirect_and_word_next_and_parenthesis_next(c))
				return (1);
		}
	}
	return (0);
}

int	check_first_token(t_token *c)
{
	if (c->type == TOKEN_PIPE || c->type == TOKEN_OR
		|| c->type == TOKEN_AND)
	{
		error_syntax(c, 1);
		return (1);
	}
	return (0);
}

bool	check_syntax(t_token *current)
{
	int			skip;
	t_syntax	syntax;

	skip = 0;
	syntax.openpar = 0;
	syntax.operator = 0;
	if (!current)
		return (0);
	if (check_first_token(current) == 1)
		return (1);
	while (current)
	{
		if (double_operator(current) == 1)
			return (1);
		if (skip > 0)
			skip--;
		if (skip == 0 && (current->type == TOKEN_OPENPAR
				|| current->type == TOKEN_CLOSEPAR))
		{
			if (check_parenthesis(current, syntax, &skip) == 1)
				return (1);
		}
		current = current->next;
	}
	return (0);
}
