/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 16:42:50 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/03 17:28:02 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*

enum s_type{
	0 NOT_DEFINE,
	1 TOKEN_WORD,
	2 TOKEN_DQUOTES,
	3 TOKEN_SQUOTES,
	4 TOKEN_AND,
	5 TOKEN_OR,
	6 TOKEN_PIPE,
	7 TOKEN_REDIRECTIN,
	8 TOKEN_REDIRECTOUT,
	9 TOKEN_HEREDOC,
	10 TOKEN_APPENDOUT,
	11 TOKEN_LIMITER,
	12 TOKEN_OPENPAR,
	13 TOKEN_CLOSEPAR,
	14 TOKEN_WHITESPACE,
	15 TOKEN_ENV,
	16 TOKEN_WILDCARD,
	17 TOKEN_COUNT,
	18 WORD_FILEIN,
	19 WORD_FILEOUT,
	20 WORD_FILEOUT_APPEND,
	21 WORD_BUILTIN,
	22 WORD_ABSPATH,
	23 WORD_CMD,
	24 WORD_OPTION, // option / argument d'une commande
	25 WORD_LIMITER,
	26 WORD_STRING,
	27 WORD_ERROR, //ERREUR (le WORD n'est pas classifie)
	28 WORD_WTF, //dans le cas ou j'ai oublie un cas
};

1 - Concatener
2 - 

*/

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

bool	check_current_parenthesis(t_token *current)
{
	bool	operator;
	int		skip_par;
	
	operator = 0;
	skip_par = -1;
	while (current)
	{
		if (is_operator(current) && skip_par == 0)
			operator = 1;
		if (current->type == TOKEN_OPENPAR)
			skip_par++;
		if (current->type == TOKEN_CLOSEPAR)
		{
			if (operator == 0 && skip_par == 0)
			{
				error_syntax(current, 4);
				return (1);
			}
			skip_par--;
		}
		current = current->next;
	}
	return (0);
}


// regler: test ((&&))
//chck_parenthesis(current, )
bool	check_parenthesis(t_token *current, t_syntax syntax, int *skip)
{
	*skip += 1;
	if (is_operator(current))
	{
		if (current->next)
		{
			syntax.operator = 1;
			return(check_parenthesis(current->next, syntax, skip));
		}
	}
	else if (current->type == TOKEN_CLOSEPAR)
	{
		if (syntax.openpar <= 0 || syntax.operator == 0 || (syntax.openpar - 1 > 0 && !current->next))
		{
			if (syntax.openpar <= 0)
				error_syntax(current, 1);
			else if (syntax.openpar - 1 > 0 && !current->next)
				error_syntax(current, 2);
			else if (syntax.operator == 0)
				error_syntax(current, 4);
			return (1);
		}
		else if (current->next)
		{
			syntax.openpar -= 1;
			return (check_parenthesis(current->next, syntax, skip));
		}
		syntax.openpar--;
	}
	else if (current->type == TOKEN_OPENPAR)
	{
		if (check_current_parenthesis(current) == 1)
			return (1);
		if (current->next)
		{
			syntax.openpar += 1;
			syntax.operator = 0;
			return (check_parenthesis(current->next, syntax, skip));
		}
		syntax.openpar++;
	}
	else if (current->next)
		return (check_parenthesis(current->next, syntax, skip));
	if (!current->next && syntax.openpar != 0)
	{
		error_syntax(current, 1);
		return (1);
	}
	return (0);
}

/* bool	redirect_error_syntax(t_token *current, t_syntax syntax, int *skip)
{
	
} */
