/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 10:56:18 by allan             #+#    #+#             */
/*   Updated: 2024/07/01 16:58:42 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_all_env(t_token **token_list)
{
	t_token	*current;

	while (*token_list && ((*token_list)->type == TOKEN_ENV))
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

void	remove_token(t_token **current, bool option)
{
	t_token	*tmp;

	if (!current || !(*current))
		return ;
	if (option == 0) //CASE First node of token_list is an ENV OK
	{
		tmp = *current;
		*current = (*current)->next;
		tmp->next = NULL;
		token_free(&tmp);
	}
	else if (option == 1)//CASE Any middle node of token_list is an ENV OK
	{
		tmp = (*current)->next;
		(*current)->next = (*current)->next->next;
		tmp->next = NULL;
		token_free(&tmp);
	}
}
