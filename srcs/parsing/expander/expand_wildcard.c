/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 21:24:08 by allan             #+#    #+#             */
/*   Updated: 2024/06/19 01:34:17 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*

		SPECIAL CASE: 
		HIDDEN FILES with "."
		
		
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
*/

int	expand_wildcard(t_token **token_list, t_token *current, int *error)
{
	t_token	*new_list;
	
	new_list = malloc(sizeof(t_token));
	if (!new_list)
		return (1);
	token_init(new_list);
	while (current)
	{
		if (current->type == TOKEN_WILDCARD)
			find_wildcard(current, &new_list, error);
		else
		{
			*error = relink_operator(current, &new_list);
			if (current)
				current = current->next;
		}
		if (*error == 1)
		{
			token_free(&new_list);
			return (1);
		}
	}
	token_free(token_list);
	*token_list = new_list;
	return (0);
}

void find_wildcard(char *wildcard, t_token **new_list, int *error)
{
	int	i;

	i = 0;
	while (/* condition */)
	{
		/* code */
	}
	
}