/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_norm.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 16:18:03 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/11 16:19:48 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	if_is_a_redirect(t_token *c)
{
	if (c->type == TOKEN_HEREDOC || c->type == TOKEN_REDIRECTIN
		|| c->type == TOKEN_REDIRECTOUT
		|| c->type == TOKEN_APPENDOUT)
		return (1);
	return (0);
}

bool	condition_of_multiple_is_ope(t_token *c)
{
	if ((is_operator(c->type, 2) && (is_operator(c->next->type, 2)
				|| is_operator(c->next->type, 4))) || (is_operator(c->type, 3)
			&& ((is_operator(c->next->type, 3) || is_operator(c->next->type, 2)
					|| is_operator(c->next->type, 4)))))
		return (1);
	else
		return (0);
}

bool	if_last_token_is_opearator(t_token *c)
{
	if ((is_operator(c->type, 2) || is_operator(c->type, 3)))
	{
		error_syntax(c, 5);
		return (1);
	}
	return (0);
}

bool	redirect_and_word_next_and_parenthesis_next(t_token *c)
{
	if ((c->type == TOKEN_REDIRECTIN || c->type == TOKEN_REDIRECTOUT)
		&& c->next->type == TOKEN_WORD && c->next->next->type == TOKEN_OPENPAR)
	{
		error_syntax(c->next->next, 1);
		return (1);
	}
	return (0);
}

int	triple_if_in_double_operator(t_token *c)
{
	if (c->type == TOKEN_HEREDOC
		&& (c->next->type != WORD_LIMITER && c->next->type != WORD_SQLIMITER))
	{
		error_syntax(c->next, 1);
		return (1);
	}
	if ((c->type == TOKEN_REDIRECTIN || c->type == TOKEN_REDIRECTOUT)
		&& c->next->type != TOKEN_WORD)
	{
		error_syntax(c->next, 1);
		return (1);
	}
	if (c->type == TOKEN_REDIRECTIN && c->next->type == TOKEN_REDIRECTOUT)
	{
		error_syntax(c, 5);
		return (1);
	}
	return (0);
}
