/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   relink_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 23:11:11 by allan             #+#    #+#             */
/*   Updated: 2024/07/11 18:25:25 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	relink_token(t_token **token_list, t_token *c, int error)
{
	t_token	*new_list;

	new_list = malloc(sizeof(t_token));
	if (!new_list)
		return (1);
	token_init(&new_list);
	while (c)
	{
		error = 1;
		if ((c->state == STATE_WORD || c->state == STATE_EXIT_STATUS)
			&& (c->type != WORD_LIMITER && c->type != WORD_SQLIMITER))
			c = relink_word(c, &new_list, &error);
		else
		{
			error = relink_operator(c, &new_list);
			if (c)
				c = c->next;
		}
		if (error == 1)
			return (token_free(&new_list));
	}
	token_free(token_list);
	*token_list = new_list;
	return (0);
}

t_token	*relink_word(t_token *c, t_token **new_list, int *error)
{
	char	*word;
	char	*new_word;
	int		wildcard;

	relink_word_init(&word, &new_word, &wildcard);
	while (c && (c->state == STATE_WORD || c->state == STATE_EXIT_STATUS))
	{
		if (c->state == STATE_EXIT_STATUS)
			wildcard = 2;
		if (c->type == TOKEN_WILDCARD)
			wildcard = 1;
		if (!word)
			new_word = ft_strdup(c->value);
		else
			new_word = ft_strjoin(word, c->value);
		free(word);
		if (!new_word)
			return (NULL);
		word = new_word;
		c = c->next;
	}
	if (add_word(new_list, word, wildcard) == 1)
		return (NULL);
	*error = 0;
	return (c);
}

void	relink_word_init(char **word, char **new_word, int *wildcard)
{
	*word = NULL;
	*new_word = NULL;
	*wildcard = 0;
}

bool	add_word(t_token **new_list, char *word, int option)
{
	t_token	*last;

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
	else if (option == 2)
	{
		last->state = STATE_EXIT_STATUS;
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
	t_token	*last;

	if (current->state == STATE_OPERATOR)
	{
		if (token_addback(new_list, current->value, 2) == 1)
			return (1);
		last = token_last(*new_list);
		last->state = current->state;
		last->type = current->type;
	}
	else if (current->type == WORD_LIMITER || current->type == WORD_SQLIMITER)
	{
		if (token_addback(new_list, current->value, 2) == 1)
			return (1);
		last = token_last(*new_list);
		last->state = current->state;
		last->type = current->type;
	}
	return (0);
}
