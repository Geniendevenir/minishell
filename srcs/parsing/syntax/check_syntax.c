/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 16:42:50 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/04 12:59:54 by allan            ###   ########.fr       */
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

bool	is_operator(t_token *c, int option)
{
	if (option == 1)
	{
		if (c->type == TOKEN_AND || c->type == TOKEN_OR)
			return (1);
	}
	if (option == 2)
	{
		if (c->type == TOKEN_AND || c->type == TOKEN_OR
			|| c->type == TOKEN_PIPE)
			return (1);
	}
	if (option == 3)
	{
		if (c->type == TOKEN_REDIRECTIN || c->type == TOKEN_REDIRECTOUT
			|| c->type == TOKEN_APPENDOUT || c->type == TOKEN_HEREDOC)
			return (1);
	}
	if (option == 4)
	{
		if (c->type == TOKEN_OPENPAR || c->type == TOKEN_CLOSEPAR)
			return (1);
	}
	return (0);
}
/* REGLER PROBLEME:
1
bash: syntax error near unexpected token '&&'
Minishell> test (&&


*/
int		double_operator(t_token *c)
{
	if (!c->next)
	{
		if ((is_operator(c, 2) || is_operator(c, 3)))
		{
			error_syntax(c, 5);
			return (1);
		}
	}
	else if (c->next)
	{
		if (c->type == TOKEN_REDIRECTIN && c->next->type == TOKEN_REDIRECTOUT)
		{
			error_syntax(c, 5);
			return (1);
		}
		if ((is_operator(c, 2) && (is_operator(c->next, 2)
			|| is_operator(c->next, 4))) || (is_operator(c, 3)
			&& ((is_operator(c->next, 3) || is_operator(c->next, 2)
			|| is_operator(c->next, 4)))))
		{
			error_syntax(c->next, 1);
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

bool 	check_syntax(t_token *current)
{
	if (!current)
		return (0);
	int			skip;
	t_syntax	syntax;
	
	skip = 0;
	syntax.openpar = 0;
	syntax.operator = 0;
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
