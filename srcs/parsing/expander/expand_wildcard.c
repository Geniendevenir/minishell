/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 21:24:08 by allan             #+#    #+#             */
/*   Updated: 2024/06/19 23:18:38 by allan            ###   ########.fr       */
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

int	expand_wildcard(t_token **token_list, int *error)
{
	t_token *current;
	char	*pattern;
	
	pattern = NULL;
	*error = 1;
	current = *token_list;
	while (current)
	{
		if (current->type == TOKEN_WILDCARD)
		{
			pattern = ft_strdup(current->value);
			if (!pattern)
				return (1);
			error = find_wildcard(pattern, current, error);
			free(pattern);
		}
		else // besoin du else ?
			current = current->next;
	}
	return (0);
}

int find_wildcard(char *pattern, t_token *current, int *error)
{
	DIR				*d;
    struct dirent	*dir;
	bool			found;
	
	found = 0;
    d = opendir(".");
    if (!d)
	{
        perror("opendir");
        return (1); //add error
    }
    while ((dir = readdir(d)) != NULL)
	{
        if (file_match(pattern, dir->d_name, error))
		{
            error = add_file(&current, dir->d_name, found);
			found = 1;
		}
		if (error == 1)
			return (1); //add error
    }
    closedir(d);
	return (0);
}

bool	file_match(char *pattern, char *file_name, int *error)
{
	return (0);
}

bool	add_file(t_token **current, char *file_name, bool found)
{
	if (found == 0)
	{
		free((*current)->value);
		(*current)->value = ft_strdup(file_name);
		if (!(*current)->value)
			return (1);
	}
	else
	{
		if (token_addnext(current, file_name) == 1)
			return (1);
		(*current) = (*current)->next;
	}
	(*current)->type = TOKEN_WORD;
	return (0);
}
