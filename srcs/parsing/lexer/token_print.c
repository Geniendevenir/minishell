/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 16:30:01 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/17 18:45:47 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	int	i;

	i = 0;
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