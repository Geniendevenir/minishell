/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 17:22:12 by allan             #+#    #+#             */
/*   Updated: 2024/06/14 19:26:25 by allan            ###   ########.fr       */
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

bool	token_addback(t_token **token_list, char *value, bool option)
{
	t_token *token;
	t_token	*last_token;
	
	/* if (!token_list) //check si possible de supprimer
		return (1); */
	token = malloc(sizeof(t_token));
	if (!token)
		return (1);
	token_init(token);
	token->value = ft_strdup(value);
	if (!token->value)
		return (1);
	if (is_freeable(token->value, option) == 0)
		free(value);
	token->next = NULL;
	if ((*token_list)->value == NULL)
	{
		last_token = *token_list;
		*token_list = token;
		free(last_token);
	}
	else
	{
		last_token = token_last(*token_list);
		last_token->next = token;
	}
	return (0);
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
	printf("NULL\n\n\n");
}
void	token_print_amazing(t_token **token_list)
{
	t_token	*current;

	int i = 0;
	if (!token_list)
		return ;
	current = *token_list;
	while (current)
	{
		if (current->value)
			amazing_printing(current, i);
		current = current->next;
		i++;
	}
	printf("NULL\n");
}

void	amazing_printing(t_token *current, int i)
{
	printf("----------------------------------\n");
    printf("| Node: %d                        |\n", i);
    printf("----------------------------------\n");
    printf("| Value: %s                        \n", current->value);
    printf("----------------------------------\n");
    printf("| Type: %s                        \n", getToken_Class(current));
    printf("----------------------------------\n");
    printf("| Len: %ld                        |\n", current->len);
    printf("----------------------------------\n");
}

const char* getToken_Class(t_token *current) {
	const char* Token_Class[] = {
    	"NOT_DEFINE",
		"TOKEN_WORD",
		"TOKEN_DQUOTES",
		"TOKEN_SQUOTES",
		"TOKEN_AND",
		"TOKEN_OR",
		"TOKEN_PIPE",
		"TOKEN_REDIRECTIN",
		"TOKEN_REDIRECTOUT",
		"TOKEN_HEREDOC",
		"TOKEN_APPENDOUT",
		"TOKEN_LIMITER",
		"TOKEN_OPENPAR",
		"TOKEN_CLOSEPAR",
		"TOKEN_WHITESPACE",
		"TOKEN_ENV",
		"TOKEN_WILDCARD",
		"TOKEN_COUNT"
	};
    if (current->type >= 0 && current->type < TOKEN_COUNT) {
        return Token_Class[current->type];
    } else {
        return "NULL";
    }
	
}
//printf("%s->", current->value);
/*
token_remove()
token_join()
*/

