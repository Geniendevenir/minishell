/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 16:47:23 by allan             #+#    #+#             */
/*   Updated: 2024/06/08 12:28:51 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
		WILDCARD CASES:

		CASE 1:	Hello*.txt
		-> TOKEN_WILDCARD
		
		CASE 2:	*.txt
		-> TOKEN_WILDCARD
		
		CASE 3:	$USER*.txt
		-> TOKEN_ENV: $USER
		-> TOKEN_WILDCARD: *.txt
		
		CASE 4:	*$USER.txt
		-> TOKEN_WILDCARD: *
		-> TOKEN_ENV: $USER
		-> TOKEN_WORD: .txt
		
		CASE 5:	*Hello$USER
		-> TOKEN_WILDCARD: *Hello
		-> TOKEN_ENV: $USER

		CASE 6: *mini*****c
		-> TOKEN_WILDCARD

		CASE 7: *mini*c****
		-> TOKEN_WILDCARD

		CASE 8: test$USER*hello.txt
		-> TOKEN_WORD: test
		-> TOKEN_ENV: $USER
		-> TOKEN_WILDCARD: *hello.txt

		CASE 9: test$USERHello*txt
		-> TOKEN_WORD: test
		-> TOKEN_ENV: $USERHello
		-> TOKEN_WILDCARD: *.txt

		CASE 10: test$USERHello*$USER.txt.$USER
		-> TOKEN_WORD: test
		-> TOKEN_ENV: $USERHello
		-> TOKEN_WILDCARD: *
		-> TOKEN_ENV: $USER
		-> TOKEN_WORD: .txt
		-> TOKEN_ENV: $USER

	ETAPE 1: Creer un TOKEN_WILDCARD / UNE REDIRECTION TOKEN_WORD VERS TOKEN_WILDCARD
	ETAPE 2: EXPAND ENV. IF FOUND -> REPLACE NEW VALUE. IF NOT FOUND -> DELETE THE TOKEN. 
	ETAPE 3: RECOLLER LES WORD/ENV/WILDCARD SI il n'y a pas d'espaces entre
	ETAPE 4: EXPAND WILDCARD
	FIN

	ENV:

	BIEN REGARDER CE CAS
	test$USERhello*txt
	testallanhello.txt: command not found
*/

/*
	FIND_ENV

	C'est le tout premier node et:
	CASE 1: On trouve l'env
	CASE 2: On trouve pas l'env

	C'est un node random:
	CASE 1: On trouve l'env
	CASE 2: On trouve pas l'env

	C'est le dernier node
	CASE 1: On trouve l'env
	CASE 2: On trouve pas l'env 
*/

bool	expander(t_token **token_list, t_env *env)
{
	if (!env)
	{
		remove_all_env(token_list);
		return (0);
	}
	if (expand_env(token_list, &env) == 1) // Expand ENV First
	{
		error_lexer(1); //error malloc
		return (1);
	}
	//relink_token();
	//expand_wildcard(); //Then expand Wild Card
	return (0);
}

//printf("test1\n");

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
		if (current->next->type == TOKEN_ENV) //
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

bool	replace_token(t_token *token,  char *new_value) //OK
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