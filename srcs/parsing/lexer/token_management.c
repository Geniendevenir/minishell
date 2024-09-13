/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 17:22:12 by allan             #+#    #+#             */
/*   Updated: 2024/09/12 17:42:01 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	token_init(t_token **token_list)
{
	(*token_list)->state = STATE_START;
	(*token_list)->type = NOT_DEFINE;
	(*token_list)->value = NULL;
	(*token_list)->len = 0;
	(*token_list)->next = NULL;
}

t_token	*token_last(t_token *token_list)
{
	while (token_list)
	{
		if (!token_list->next)
			return (token_list);
		token_list = token_list->next;
	}
	return (token_list);
}

bool	token_addback(t_token **token_list, char *value, int option)
{
	t_token	*token;

	if (!token_list)
	{
		if (is_freeable(value, option) == 0)
			free(value);
		return (1);
	}
	token = malloc(sizeof(t_token));
	if (!token)
	{
		if (is_freeable(value, option) == 0)
			free(value);
		return (1);
	}
	token_init(&token);
	token->value = ft_strdup(value);
	if (is_freeable(value, option) == 0)
		free(value);
	if (!token->value)
		return (1);
	token->next = NULL;
	token_addback_two(token_list, token);
	return (0);
}

void	token_addback_two(t_token **token_list, t_token *token)
{
	t_token	*last_token;

	printf("a\n");
	/* if (!*token_list)
		return ; */
	if ((*token_list)->value == NULL)
	{
		printf("b\n");
		last_token = *token_list;
		printf("c\n");
		*token_list = token;
		printf("d\n");
		free(last_token);
	}
	else
	{
		printf("e\n");
		last_token = token_last(*token_list);
		printf("f\n");
		last_token->next = token;
	}
}

bool	token_addnext(t_token **current, char *value)
{
	t_token	*token;

	if (!current)
		return (1);
	token = malloc(sizeof(t_token));
	if (!token)
		return (1);
	token_init(&token);
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (1);
	}
	token->next = (*current)->next;
	(*current)->next = token;
	return (0);
}
