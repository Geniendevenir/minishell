/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 10:56:18 by allan             #+#    #+#             */
/*   Updated: 2024/07/11 18:41:02 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_all_env(t_token **token_list, int exit_status)
{
	t_token	*current;

	expand_all_exit(token_list, exit_status);
	while (*token_list && (*token_list)->type == TOKEN_ENV)
		remove_token(token_list, 0);
	if (!(*token_list) || !(*token_list)->next)
		return ;
	current = *token_list;
	while (current && current->next)
	{
		if (current->next->type == TOKEN_ENV)
			remove_token(&current, 1);
		else
			current = current->next;
	}
}

/*
L44 //CASE First node of token_list is an ENV OK
L51 //CASE Any middle node of token_list is an ENV OK
*/
void	remove_token(t_token **current, bool option)
{
	t_token	*tmp;

	if (!current || !(*current))
		return ;
	if (option == 0)
	{
		tmp = *current;
		*current = (*current)->next;
		tmp->next = NULL;
		token_free(&tmp);
	}
	else if (option == 1)
	{
		tmp = (*current)->next;
		(*current)->next = (*current)->next->next;
		tmp->next = NULL;
		token_free(&tmp);
	}
}

bool	expand_all_exit(t_token **token_list, int exit_status)
{
	t_token	*current;

	if (!(*token_list))
		return (0);
	if ((*token_list)->state == STATE_EXIT_STATUS)
	{
		if (replace_exit_status(*token_list, exit_status) == 1)
			return (1);
		(*token_list)->type = TOKEN_WORD;
	}
	current = *token_list;
	while (current && current->next)
	{
		if (current->next->state == STATE_EXIT_STATUS)
		{
			if (replace_exit_status(current->next, exit_status) == 1)
				return (1);
			current->next->type = TOKEN_WORD;
		}
		current = current->next;
	}
	return (0);
}
