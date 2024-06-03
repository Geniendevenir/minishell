/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 17:22:12 by allan             #+#    #+#             */
/*   Updated: 2024/06/03 19:32:08 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool token_init(t_token *token_list)
{
	/* token_list = malloc(sizeof(t_token));
	if (!token_list)
		return (1); */
	token_list->state = STATE_START;
	token_list->type = NOT_DEFINE;
	token_list->value = NULL;
	token_list->len = 0;
	token_list->next = NULL;
	return (0);
}

t_token *token_last(t_token *token_list)
{
	while (token_list)
	{
		if (!token_list->next)
			return (token_list);
		token_list = token_list->next;
	}
	return (token_list);
}

void	token_addback(t_token **token_list, char *value)
{
	t_token *token;
	t_token	*last_token;
	
	if (!token_list)
		return ;
	token = malloc(sizeof(t_token));
	if (!token)
		return ; //add error
	token_init(token);
	token->value = ft_strdup(value);
	if (!token->value) //check effet
		return ; // add error
	if (is_freeable(token->value) == 0)
		free(value);
	token->next = NULL;
	if (!(*token_list))
		*token_list = token;
	else
	{
		last_token = token_last(*token_list);
		last_token->next = token;
	}
}

void	token_free(t_token **token_list)
{
	t_token	*current;
	t_token	*tmp;

	if (!token_list)
		return ;
	current = *token_list;
	while (current)
	{
		tmp = current->next;
		if (current->value)
			free(current->value);
		free(current);
		current = tmp;
	}
	*token_list = NULL;
}

void	token_print(t_token **token_list)
{
	t_token	*current;

	if (!token_list)
		return ;
	current = *token_list;
	while (current)
	{
		if (current->value)
			printf("%s->", current->value);
		current = current->next;
	}
	printf("NULL\n");
}
/*
token_remove()
token_join()
*/

