/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 16:42:50 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/16 15:03:41 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	if_in_check_double_syntax(t_token *c, int number)
{
	if (number == 1)
	{
		if (c->type == 1 || c->type == 14 ||
		c->type == 2 || c->type == 3 || c->type == 12 || c->type == 13)
				return (1);
		else
			return (0);
	}
	else if (number == 2)
	{
		if (c->type == 4 || c->type == 5 || c->type == 6 || c->type == 7 ||
		c->type == 8 || c->type == 9 || c->type == 10)
			return (1);
		else
			return (0);
	}
	return (1);
}

int	while_of_check_double_syntax(t_token *current, int is_operator)
{
	while (current && if_in_check_double_syntax(current, 1))
		current = current->next;
	is_operator = 1;
	if (current == NULL || current->next == NULL)
		return (0);
	current = current->next;
	while (current && is_operator == 1)
	{
		if (current && if_in_check_double_syntax(current, 2))
		{
			print_error_token(current->value);
			return (1);
		}
		else if(current && current->type == TOKEN_WHITESPACE)
			current = current ->next;
		else if(current)
		{
			is_operator = 0;
			current = current->next;
		}
	}
	return (0);
}

bool	check_double_syntax(t_token **token_list)
{
	t_token	*current;
	int is_operator;

	is_operator = 0;
	current = *token_list;
	while(current && current->next)
	{
		if (while_of_check_double_syntax(current, is_operator) == 1)
		{
			return (1);
		}
		else
			return (0);
	}
	return (0);
}

bool check_operator_at_the_end(t_token **token_list)
{
	t_token	*current;
	int is_operator;

	is_operator = 0;
	current = *token_list;
	while (current)
	{
		if (current && if_in_check_double_syntax(current, 2))
			is_operator = 1;
		else if(current && current->type == TOKEN_WHITESPACE)
			;
		else if (current && if_in_check_double_syntax(current, 1))
			is_operator = 0;
		if (current->next == NULL)
			break;
		else
			current = current->next;
			
	}
	if (is_operator == 1)
	{
		print_error_token_special(current->value);
		return (1);
	}
	else
		return (0);
}

bool check_if_first_operator(t_token **token_list)
{
	t_token	*current;

	current = *token_list;
	while ((current) && current->type == TOKEN_WHITESPACE)
	{
		current = current ->next;
	}
	if (current->type == TOKEN_AND || current->type == TOKEN_OR || current->type == TOKEN_PIPE)
	{
		print_error_token(current->value);
		return (1);
	}
	else
		return (0);
}

bool check_syntax(t_token **token_list)
{
	if (check_if_first_operator(token_list) == 1)
	{
		token_free(token_list);
		return (1);
	}
	if (check_double_syntax(token_list) == 1)
	{
		token_free(token_list);
		return (1);
	}
	if (check_operator_at_the_end(token_list) == 1)
	{
		token_free(token_list);
		return (1);
	}
	return (0);
}

