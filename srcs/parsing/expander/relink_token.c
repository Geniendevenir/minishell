/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   relink_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 23:11:11 by allan             #+#    #+#             */
/*   Updated: 2024/06/16 13:46:41 by allan            ###   ########.fr       */
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
	int		error;
	
	new_list = malloc(sizeof(t_token));
	if (!new_list)
		return (1);
	token_init(new_list);
	current = *token_list;
	while (current)
	{
		if (current->state == STATE_WORD)
			current = relink_word(current, &new_list, &error);
		else
			error = relink_operator(current, &new_list);
		if (error == 1)
		{
			token_free(&new_list);
			return (1);
		}
		if (current)
			current = current->next;
	}
	token_free(token_list);
	*token_list = new_list;
	return (0);
}

t_token	*relink_word(t_token *current, t_token **new_list, int *error)
{
	char	*word;
	char	*new_word;
	t_token *last;
	
	word = NULL;
	new_word = NULL;
	*error = 1;
	while (current && current->state == STATE_WORD)
	{
		printf("relink word\n");
		if (!word)
			new_word = ft_strdup(current->value);
		else
			new_word = ft_strjoin(word, current->value);
		if (!new_word)
		{
			free(word);
			return (NULL);
		}
		free(word);
		word = new_word;
		current = current->next;
	}
	if (token_addback(new_list, word, 0) == 1)
	{
		free(word);
		return (NULL);
	}
	last = token_last(*new_list);
	last->state = STATE_WORD;
	last->type = TOKEN_WORD;
	*error = 0;
	return (current);
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
