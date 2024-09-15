/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_parenthesis.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 13:31:07 by Matprod           #+#    #+#             */
/*   Updated: 2024/09/15 16:12:41 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_parenthesis(t_token *current, t_syntax s, int *skip)
{
	int	error;

	error = 0;
	*skip += 1;
	if (*skip != 1 && s.openpar == 0)
		return (0);
	if (current->type == TOKEN_PIPE)
		return (error_syntax(current, 6), 1);
	if (is_operator(current->type, 1))
	{
		if (current->next)
			return (check_parenthesis(current->next,
					(t_syntax){.openpar = s.openpar, .operator = 1}, skip));
	}
	else if (current->type == TOKEN_CLOSEPAR || current->type == TOKEN_OPENPAR)
	{
		error = check_parenthesis_error(current, &s, skip);
		if (error != 2)
			return (error);
	}
	else if (current->next)
		return (check_parenthesis(current->next, s, skip));
	if (!current->next && s.openpar != 0)
		return (error_syntax(current, 1), 1);
	return (0);
}

bool	closepar_error(t_token *current, t_syntax syntax, int *skip)
{
	if (syntax.openpar <= 0 || syntax.operator == 0
		|| (syntax.openpar - 1 > 0 && !current->next))
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
	return (0);
}

bool	openpar_error(t_token *current, t_syntax syntax, int *skip)
{
	if (check_current_parenthesis(current, 2) == 1)
		return (1);
	else if (current->next)
	{
		syntax.openpar += 1;
		syntax.operator = 0;
		return (check_parenthesis(current->next, syntax, skip));
	}
	return (0);
}

bool	is_parenthesis_error(t_token *current, t_syntax syntax, int option)
{
	if (option == 1)
	{
		if (syntax.openpar <= 0 || syntax.operator == 0
			|| (syntax.openpar - 1 > 0 && !current->next))
			return (1);
		else if (current->next)
			return (1);
	}
	else if (option == 2)
	{
		if (check_current_parenthesis(current, 1) == 1 || current->next)
			return (1);
	}
	return (0);
}

bool	check_current_parenthesis(t_token *current, int option)
{
	bool	operator;
	int		skip_par;

	operator = 0;
	skip_par = -1;
	while (current)
	{
		if (is_operator(current->type, 1) && skip_par == 0)
			operator = 1;
		if (current->type == TOKEN_OPENPAR)
			skip_par++;
		if (current->type == TOKEN_CLOSEPAR)
		{
			if (operator == 0 && skip_par == 0)
			{
				if (option == 1)
					error_syntax(current, 4);
				return (1);
			}
			skip_par--;
		}
		current = current->next;
	}
	return (0);
}
