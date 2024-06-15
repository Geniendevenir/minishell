/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 10:52:47 by allan             #+#    #+#             */
/*   Updated: 2024/06/15 23:10:01 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		expand_env(t_token **token_list, t_env **env)
{
	t_token	*current;

	while (*token_list && ((*token_list)->type == TOKEN_ENV))
	{
		if (find_first_env(token_list, env) == 1)
			return (1);
	}
	if (!(*token_list) || !(*token_list)->next)
		return (0);
	current = *token_list;
	token_print(&current);
	while (current && current->next)
	{
		if (current->next->type == TOKEN_ENV)
		{
			if (find_next_env(&current, env) == 1)
				return (1);
		}
		else
			current = current->next;
	}
	return (0);
}

bool	find_first_env(t_token **current, t_env **env)
{
	t_env	*find;

	find = *env;
	while (find)
	{
		if (ft_strcmp((*current)->value, find->key) == 0)
		{
			if (replace_token((*current), find->value) == 1)
				return (1);
			(*current)->type = TOKEN_WORD;
			return (0);
		}
		find = find->next;
	}
	remove_token(current, 0);
	return (0);
}

bool	find_next_env(t_token **current, t_env **env)
{
	t_env	*find;

	find = *env;
	while (find)
	{
		if (ft_strcmp((*current)->next->value, find->key) == 0)
		{
			if (replace_token((*current)->next, find->value) == 1)
				return (1);
			(*current)->next->type = TOKEN_WORD;
			return (0);
		}
		find = find->next;
	}
	remove_token(current, 1);
	return (0);
}

bool	replace_token(t_token *token,  char *new_value)
{
	if (token->value == NULL)
	{
		token->value = ft_strdup(new_value);
		if (!token->value)
			return (1);
	}
	else
	{
		free(token->value);
		token->value = NULL;
		token->value = ft_strdup(new_value);
		if (!token->value)
			return (1);
	}
	return (0);
}

