/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 10:52:47 by allan             #+#    #+#             */
/*   Updated: 2024/06/08 12:26:01 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* bool	expand_quoted_env(t_token **token_list, t_env *env)
{
	t_token	*current;

	while (*token_list && ((*token_list)->type == TOKEN_DQUOTES))
	{
		if (find_first_dquotes(token_list, env) == 1)
			return (1);
	}
	if (!(*token_list) || !(*token_list)->next)
		return (0);
	current = *token_list;
	token_print(&current);
	while (current && current->next)
	{
		if (current->next->type == TOKEN_DQUOTES)
		{
			if (find_next_env(&current, env) == 1)
				return (1);
		}
		else
			current = current->next;
	}
	return (0);
} */



