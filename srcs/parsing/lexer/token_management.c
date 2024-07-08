/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 17:22:12 by allan             #+#    #+#             */
/*   Updated: 2024/07/07 18:58:12 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void token_init(t_token **token_list)
{
	(*token_list)->state = STATE_START;
	(*token_list)->type = NOT_DEFINE;
	(*token_list)->value = NULL;
	(*token_list)->len = 0;
	(*token_list)->next = NULL;
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

bool	token_addback(t_token **token_list, char *value, int option)
{
	t_token *token;

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
}

bool	token_addnext(t_token **current, char *value)
{
	t_token *token;

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

int	token_free(t_token **token_list)
{
	t_token	*current;
	t_token	*tmp;

	if (!token_list)
		return (1);
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
	return (1);
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
	printf("| State: %s                        \n", getToken_State(current));
	printf("----------------------------------\n");
}

const char* getToken_State(t_token *current)
{
	const char* Token_Class[] = {
    	"STATE_START",
		"STATE_WHITESPACE",
		"STATE_WORD",
		"STATE_OPERATOR",
		"STATE_EXIT_STATUS"
	};
    if (current->state >= 0 && current->state <= 4) {
        return Token_Class[current->state];
    } else {
        return "NULL";
    }
}

const char* getToken_Class(t_token *current)
{
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
		"TOKEN_COUNT",
		"WORD_FILEIN",
		"WORD_FILEOUT",
		"WORD_FILEOUT_APPEND",
		"WORD_BUILTIN",
		"WORD_ABSPATH",
		"WORD_CMD",
		"WORD_OPTION",
		"WORD_LIMITER",
		"WORD_STRING",
		"WORD_ERROR",
		"WORD_WTF",
		"WORD_SQLIMITER"
	};
    if (current->type >= 0 && current->type <= 29) {
        return Token_Class[current->type];
    } else {
        return "NULL";
    }
}
