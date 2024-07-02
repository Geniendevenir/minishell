/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 16:47:23 by allan             #+#    #+#             */
/*   Updated: 2024/07/01 19:53:37 by allan            ###   ########.fr       */
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

		CASE 11: (presence d'un fichier nomme: tet)
		-> COMMAND: cd te*et
		-> Ne trouve pas les palindrome: bash: cd: te*et: No such file or directory

		CASE 12: (presence fichier: tet et test*txt)
		-> COMMAND: cd te**
		-> Retour: bash: cd: too many arguments (trouve les deux fichiers)

		CASE 13: (presence fichier: *ok)
		-> COMMAND: cd *o
		->Retour: bash: cd: *o: No such file or directory (ne finit pas par o mais par k)

	ETAPE 1: Creer un TOKEN_WILDCARD / UNE REDIRECTION TOKEN_WORD VERS TOKEN_WILDCARD
	ETAPE 2: EXPAND ENV. IF FOUND -> REPLACE NEW VALUE. IF NOT FOUND -> DELETE THE TOKEN. 
	ETAPE 3: RECOLLER LES WORD/ENV/WILDCARD SI il n'y a pas d'espaces entre
	ETAPE 4: EXPAND WILDCARD
	POUR WILDCARD:
	1 - (ex: BEGGIN*END) REGARDER SI DOSSIER COMMANCANT STRICTEMENT PAR BEGIN ET FINISSANT PAR END
	2 - SI EXISTE REMPLACER PAR LE NOM DU FICHIER
	3 - SINON GARDER LA SYNTAX DE BASE BEGGIN*END
	FIN

	ENV:

	BIEN REGARDER CE CAS
	test$USERhello*txt
	test*txt: command not found
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


//CHECK ERROR ENTRE RELINK ET WILDCARD
bool	expander(t_token **token_list, t_env *env, int error)
{
	t_token	*current;

	if (!env)
		remove_all_env(token_list);
	else if (expand_env(token_list, &env) == 1)
	{
		error_lexer(1);
		token_free(token_list);
		return (1);
	}
	error = 1;
	current = *token_list;
	if (relink_token(token_list, current, error) == 1)
	{
		error_lexer(1);
		token_free(token_list);
		return (1);
	}
	if (expand_wildcard(token_list, &error) == 1)
	{
		error_lexer(error);
		token_free(token_list);
		return (1);
	}
	return (0);
}


