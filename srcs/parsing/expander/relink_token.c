/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   relink_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 23:11:11 by allan             #+#    #+#             */
/*   Updated: 2024/06/16 00:15:47 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
0 - Creer une nouvelle struct
1 - Parcourir token list
2 - Si Type == WORD tant que type == WORD
3 - else passer au token suivant
*/

int		relink_token(t_token **token_list)
{
	t_token	*new_list;
	t_token	*current;
	
	new_list = malloc(sizeof(t_token));
	if (!new_list)
		return (1);
	token_init(new_list);
	current = *token_list;
	while (current)
	{
		if (relink_word(current, &new_list) == 1)
		{
			token_free(&new_list);
			return (1);
		}
		else
		{
			if (relink_operator(current, &new_list) == 1)
			{
				token_free(&new_list);
				return (1);
			}
			current = current->next;
		}
	}
	token_free(token_list);
	token_list = &new_list;
	return (0);
}

bool	relink_word(t_token *current, t_token **new_list)
{
	char	*word;
	t_token *last;
	
	while (current && current->state == STATE_WORD)
	{
		if (!word)
		{
			word = ft_strdup(current->value);
			if (!word)
				return (1);
		}
		else
		{
			word = ft_strjoin(word, current->value);
			if (!word)
				return (1);
		}
		current = current->next;
	}	
	if (token_addback(&new_list, word, 0) == 1)
		return (1);
	last = token_last(*new_list);
	last->state = STATE_WORD;
	last->type = TOKEN_WORD;
	return (0);
}

bool	relink_operator(t_token *current, t_token **new_list)
{
	t_token *last;
	
	if (current->state == STATE_OPERATOR)
	{
		if (token_addback(&new_list, current->value, 2) == 1)
			return (1);
		last = token_last(*new_list);
		last->state = current->state;
		last->type = current->type;
	}
	return (0);
}
