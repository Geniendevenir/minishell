/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_parenthesis.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 13:31:07 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/20 17:33:28 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool check_parenthesis_number(t_token *token_list)
{
	int count;
	t_token *current;

	current = token_list;
	count = 0;
	while (current)
	{
		if (current->type = TOKEN_OPENPAR)
		{
			count++;
		}
		else if (current->type = TOKEN_CLOSEPAR)
		{
			count--;
		}
		current = current->next;
	}
	if (count != 0)
		return (1);
	else
		return (0);
}

bool check_parenthesis(t_token *token_list)
{
	
	int nb_cmd = 0;
	int operator = 0;
	t_token *current;

	current = token_list;
	while (current && current->type != TOKEN_CLOSEPAR)
	{
		if (current->next && current->type == TOKEN_OPENPAR)
		{
			current = current->next;
			return (check_parenthesis(current));
		}
		else if (current->type == WORD_CMD)
		{
			nb_cmd++;
		}
		
		current = current->next;
	}
}