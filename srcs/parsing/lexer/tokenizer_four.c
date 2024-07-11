/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_four.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 19:29:39 by allan             #+#    #+#             */
/*   Updated: 2024/07/11 16:42:20 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	create_limiter(const char *cmd_line, t_index *index, char **token_value)
{
	if (cmd_line[index->j] == '\'')
	{
		index->special = 1;
		index->error = limiter_squote(cmd_line, index, token_value);
	}
	else if (cmd_line[index->j] == '\"')
		index->error = limiter_dquote(cmd_line, index, token_value);
	else
		index->error = limiter_word(cmd_line, index, token_value);
	if (index->error == 1)
	{
		if (*token_value)
			free(*token_value);
		return (1);
	}
	return (0);
}

bool	limiter_token(const char *cmd_line, size_t *i, t_token **token_list)
{
	t_token	*current;
	char	*token_value;
	t_index	index;

	token_value = NULL;
	index_init(&index, i, 1);
	while (cmd_line[*index.i] && stop_limiter(cmd_line, index.i, 0) == 0)
	{
		if (create_limiter(cmd_line, &index, &token_value))
			return (1);
	}
	if (!token_value || !*token_value)
		index.error = token_addback(token_list, "\0", 2);
	else
		index.error = token_addback(token_list, token_value, 3);
	if (index.error == 1)
		return (1);
	current = token_last(*token_list);
	if (index.special == 1)
		current->type = WORD_SQLIMITER;
	else
		current->type = WORD_LIMITER;
	current->state = STATE_WORD;
	return (0);
}

bool	limiter_squote(const char *cmd_line, t_index *index, char **token_value)
{
	(*index->i) = index_foward(&index->j);
	if (!cmd_line[index->j])
		return (1);
	if (cmd_line[index->j] == '\'')
	{
		(*index->i) = index_foward(&index->j);
		return (0);
	}
	while (cmd_line[index->j] && cmd_line[index->j] != '\'')
		index->j++;
	if (!(*token_value))
	{
		*token_value = ft_substr(cmd_line, (*index->i), index->j - (*index->i));
		if (!(*token_value))
			return (1);
	}
	else
	{
		if (limiter_join(cmd_line, index, token_value) == 1)
			return (1);
	}
	(*index->i) = index_foward(&index->j);
	return (0);
}

bool	limiter_dquote(const char *cmd_line, t_index *index, char **token_value)
{
	(*index->i) = index_foward(&index->j);
	if (!cmd_line[index->j])
		return (1);
	if (cmd_line[index->j] == '\"')
	{
		(*index->i) = index_foward(&index->j);
		return (0);
	}
	while (cmd_line[index->j] && cmd_line[index->j] != '\"')
		index->j++;
	if (!(*token_value))
	{
		*token_value = ft_substr(cmd_line, (*index->i), index->j - (*index->i));
		if (!(*token_value))
			return (1);
	}
	else
	{
		if (limiter_join(cmd_line, index, token_value) == 1)
			return (1);
	}
	(*index->i) = index_foward(&index->j);
	return (0);
}

bool	limiter_word(const char *cmd_line, t_index *index, char **token_value)
{
	while (cmd_line[index->j] && stop_limiter(cmd_line, &index->j, 1) == 0)
		index->j++;
	if (!(*token_value))
	{
		*token_value = ft_substr(cmd_line, (*index->i), index->j - (*index->i));
		if (!(*token_value))
			return (1);
	}
	else
	{
		if (limiter_join(cmd_line, index, token_value) == 1)
			return (1);
	}
	(*index->i) = index->j;
	return (0);
}
