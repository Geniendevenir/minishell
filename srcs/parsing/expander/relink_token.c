/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   relink_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 23:11:11 by allan             #+#    #+#             */
/*   Updated: 2024/06/19 23:16:05 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
0 - Creer une nouvelle struct
1 - Parcourir token list
2 - Si Type == WORD tant que type == WORD
3 - else passer au token suivant
*/

int		relink_token(t_token **token_list, t_token *current, int *error)
{
	t_token	*new_list;
	
	new_list = malloc(sizeof(t_token));
	if (!new_list)
		return (1);
	token_init(new_list);
	while (current)
	{
		if (current->state == STATE_WORD)
			current = relink_word(current, &new_list, error);
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

t_token	*relink_word(t_token *current, t_token **new_list, int *error)
{
	char	*word;
	char	*new_word;
	bool	wildcard;
	
	relink_word_init(&word, &new_word, &wildcard);
	while (current && current->state == STATE_WORD)
	{
		if (current->type == TOKEN_WILDCARD)
			wildcard = 1;
		if (!word)
			new_word = ft_strdup(current->value);
		else
			new_word = ft_strjoin(word, current->value);
		free(word);
		if (!new_word)
			return (NULL);
		word = new_word;
		current = current->next;
	}
	if (add_word(new_list, word, wildcard) == 1)
		return (NULL);
	*error = 0;
	return (current);
}

void	relink_word_init(char **word, char **new_word, bool *wildcard)
{
	*word = NULL;
	*new_word = NULL;
	*wildcard = 0;
}

bool	add_word(t_token **new_list, char *word, bool option)
{
	t_token *last;
	
	if (token_addback(new_list, word, 0) == 1)
	{
		free(word);
		return (1);
	}
	last = token_last(*new_list);
	if (option == 0)
	{
		last->state = STATE_WORD;
		last->type = TOKEN_WORD;
	}
	else
	{
		last->state = STATE_WORD;
		last->type = TOKEN_WILDCARD;
	}
	return (0);
}

bool	relink_operator(t_token *current, t_token **new_list)
{
	t_token *last;
	
	if (current->state == STATE_OPERATOR)
	{
		if (token_addback(new_list, current->value, 2) == 1)
			return (1);
		last = token_last(*new_list);
		last->state = current->state;
		last->type = current->type;
	}
	return (0);
}
