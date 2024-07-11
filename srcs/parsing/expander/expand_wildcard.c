/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 21:24:08 by allan             #+#    #+#             */
/*   Updated: 2024/07/11 18:29:38 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
printf("test 1\n");
*/

int	expand_wildcard(t_token **token_list, int *error)
{
	t_token	*current;
	char	*wildcard;

	wildcard = NULL;
	*error = 1;
	current = *token_list;
	while (current)
	{
		if (current->type == TOKEN_WILDCARD)
		{
			wildcard = ft_strdup(current->value);
			if (!wildcard)
				return (1);
			*error = find_wildcard(wildcard, current, error);
			free(wildcard);
			if (*error != 0)
				return (*error);
		}
		current = current->next;
	}
	return (0);
}

int	find_wildcard(char *wildcard, t_token *current, int *error)
{
	DIR				*d;
	struct dirent	*dir;
	t_wildcard		match;
	bool			found;

	d = opendir(".");
	if (!d)
		return (6);
	found = 0;
	while (1)
	{
		dir = readdir(d);
		if (dir == NULL)
			break ;
		match_init(wildcard, dir->d_name, &match);
		if (file_match(match))
		{
			*error = add_file(&current, dir->d_name, found);
			if (*error == 1)
				return (wildcard_return(&d));
			found = 1;
		}
	}
	closedir(d);
	return (0);
}

void	match_init(char *wildcard, char *file_name, t_wildcard *match)
{
	match->file_name = file_name;
	match->wildcard = wildcard;
	match->star = NULL;
	match->backtrack = file_name;
}

bool	file_match(t_wildcard match)
{
	while (*match.file_name)
	{
		if (*match.wildcard == '*')
		{
			match.star = match.wildcard++;
			match.backtrack = match.file_name;
		}
		else if (*match.wildcard == *match.file_name || *match.wildcard == '?')
		{
			match.wildcard++;
			match.file_name++;
		}
		else if (match.star)
		{
			match.wildcard = match.star + 1;
			match.file_name = ++match.backtrack;
		}
		else
			return (0);
	}
	while (*match.wildcard == '*')
		match.wildcard++;
	return (!(*match.wildcard));
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
