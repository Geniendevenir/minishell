/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 10:52:47 by allan             #+#    #+#             */
/*   Updated: 2024/07/11 18:30:03 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//printf("test\n");
int	expand_env(t_token **token_list, t_env **env, int exit_status)
{
	t_token	*current;

	while (*token_list && (((*token_list)->type == TOKEN_ENV)))
	{
		if (find_first_env(token_list, env, exit_status) == 1)
			return (1);
	}
	if (!(*token_list) || !(*token_list)->next)
		return (0);
	current = *token_list;
	while (current && current->next)
	{
		if (current->next->type == TOKEN_ENV)
		{
			if (find_next_env(&current, env, exit_status) == 1)
				return (1);
		}
		else
			current = current->next;
	}
	return (0);
}

bool	find_first_env(t_token **current, t_env **env, int exit_status)
{
	t_env	*find;

	find = *env;
	if ((*current)->state == STATE_EXIT_STATUS)
	{
		if (replace_exit_status(*current, exit_status) == 1)
			return (1);
		(*current)->type = TOKEN_WORD;
		return (0);
	}
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

bool	find_next_env(t_token **current, t_env **env, int exit_status)
{
	t_env	*find;

	find = *env;
	if ((*current)->next->state == STATE_EXIT_STATUS)
	{
		if (replace_exit_status((*current)->next, exit_status) == 1)
			return (1);
		(*current)->next->type = TOKEN_WORD;
		return (0);
	}
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

bool	replace_token(t_token *token, char *new_value)
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

bool	replace_exit_status(t_token *token, int exit_status)
{
	if (token->value)
		free(token->value);
	token->value = ft_itoa(exit_status);
	if (!token->value)
		return (1);
	return (0);
}
