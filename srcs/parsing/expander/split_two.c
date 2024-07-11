/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 21:53:41 by allan             #+#    #+#             */
/*   Updated: 2024/07/11 19:29:12 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	dquotes_token(const char *cmd_line, size_t *i, t_token **token_list)
{
	t_index	index;

	index_init(&index, i, 0);
	(*index.i)++;
	index.j = *index.i;
	while (cmd_line[index.j] && cmd_line[index.j] != '\"')
	{
		if (cmd_line[index.j] == '$' && is_env(cmd_line[index.j + 1]) == 0)
		{
			if (env_dquotes(cmd_line, &index, token_list) == 1)
				return (1);
		}
		if (cmd_line[*index.i] == '$' && is_env(cmd_line[*index.i + 1]) == 0)
			(*index.i)++;
		else if (cmd_line[index.j] != '\"')
			index.j++;
	}
	if (dquotes_last_token(cmd_line, &index, token_list) == 1)
		return (1);
	*i = index.j + 1;
	return (0);
}

/*
L54  //Tokenize la string avant l'ENV
L57 //Se positionne apres le $ (Norminette BS)
L58 //Cas special "$?" A MODIFIER SE POSITIONNER A I--
L65 //Tous les autres cas
*/
bool	env_dquotes(const char *cmd_line, t_index *index, t_token **token_list)
{
	char	*token_value;
	int		option;

	option = 1;
	if (index->j > *index->i)
	{
		token_value = ft_substr(cmd_line, *index->i, (index->j - *index->i));
		if (in_if_envdquotes(token_value, token_list))
			return (1);
	}
	(*index->i) = index_foward(&index->j);
	if (cmd_line[index->j] == '?')
	{
		index->j++;
		option = 2;
	}
	else
	{
		while (is_valid_env(cmd_line[index->j]) == 0)
			index->j++;
	}
	token_value = ft_substr(cmd_line, *index->i, (index->j - *index->i));
	if (end_of_env_dquotes(token_value, token_list, option))
		return (1);
	(*index->i) = index->j;
	return (0);
}

bool	dquotes_last_token(const char *cmd_line, t_index *i, t_token **tl)
{
	char	*token_value;

	if (i->j > *i->i && cmd_line[*i->i] != '\"')
	{
		token_value = ft_substr(cmd_line, *i->i, (i->j - *i->i));
		if (!token_value)
			return (1);
		if (dquote_add_token(token_value, tl, 0) == 1)
			return (1);
	}
	return (0);
}

bool	dquote_add_token(char *token_value, t_token **token_list, int option)
{
	t_token	*current;
	int		len;

	len = ft_strlen(token_value);
	if (token_addback(token_list, token_value, 0) == 1)
		return (1);
	current = token_last(*token_list);
	current->len = len;
	if (option == 0)
	{
		current->state = STATE_WORD;
		current->type = TOKEN_DQUOTES;
	}
	else if (option == 1)
	{
		current->state = STATE_WORD;
		current->type = TOKEN_ENV;
	}
	else if (option == 2)
	{
		current->state = STATE_EXIT_STATUS;
		current->type = TOKEN_DQUOTES;
	}
	return (0);
}

bool	squote_token(const char *cmd_line, size_t *i, t_token **token_list)
{
	t_token		*current;
	char		*token_value;
	size_t		j;

	j = *i + 1;
	if (cmd_line[j] == '\'')
	{
		(*i) += 2;
		return (0);
	}
	while (cmd_line[j] && cmd_line[j] != '\'')
		j++;
	token_value = ft_substr(cmd_line, (*i) + 1, (j - *i) - 1);
	if (!token_value)
		return (1);
	if (token_addback(token_list, token_value, 0) == 1)
		return (1);
	current = token_last(*token_list);
	current->len = (j - *i);
	current->state = STATE_WORD;
	current->type = TOKEN_SQUOTES;
	(*i) = j + 1;
	return (0);
}
