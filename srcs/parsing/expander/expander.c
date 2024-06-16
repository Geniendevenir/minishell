/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 16:47:23 by allan             #+#    #+#             */
/*   Updated: 2024/06/16 15:44:04 by allan            ###   ########.fr       */
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
	int error;

	error = 0;
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
	printf("\nAfter expand_env:\n");
	token_print(token_list);
	if (relink_token(token_list, &error) == 1)
	{
		error_lexer(1); //error malloc
		return (1);
	}
	//expand_wildcard(); //Then expand Wild Card
	return (0);
}

//printf("test1\n");
