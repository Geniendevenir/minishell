/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_four.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 14:02:01 by allan             #+#    #+#             */
/*   Updated: 2024/07/07 19:12:22 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

bool	dquotes_token(const char *cmd_line, size_t *i, t_token **token_list)
{
	t_index	index;

	index.i = i;
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

bool	env_dquotes(const char *cmd_line, t_index *index, t_token **token_list)
{
	char	*token_value;
	int		option;

	option = 1;
	if (index->j > *index->i) //Tokenize la string avant l'ENV
	{
		token_value = ft_substr(cmd_line, *index->i, (index->j - *index->i));
		if (!token_value)
			return (1);
		if (dquote_add_token(token_value, token_list, 0) == 1) //substr de i + 1 index->jusqu'a index->j
			return (1);
	}
	(*index->i) = index_foward(&index->j); //Se positionne apres le $ (Norminette BS)
	if (cmd_line[index->j] == '?') //Cas special "$?" A MODIFIER SE POSITIONNER A I--
	{
		option = 2;
		index->j++;
	}
	else
	{
		while (is_valid_env(cmd_line[index->j]) == 0) //Tous les autres cas
			index->j++;
	}
	token_value = ft_substr(cmd_line, *index->i, (index->j - *index->i));
	if (!token_value)
		return (1);
	if (dquote_add_token(token_value, token_list, option) == 1)
		return (1);
	(*index->i) = index->j;
	return (0);
}

size_t	index_foward(size_t *j)
{
	*j += 1;
	return (*j);
}

bool	dquotes_last_token(const char *cmd_line, t_index *index, t_token **token_list)
{
	char	*token_value;
	
	if (index->j > *index->i && cmd_line[*index->i] != '\"')
	{
		token_value = ft_substr(cmd_line, *index->i, (index->j - *index->i));
		if (!token_value)
			return (1);
		if (dquote_add_token(token_value, token_list, 0) == 1)
			return (1);
	}
	return (0);
}

bool	dquote_add_token(char *token_value, t_token **token_list, int option)
{
	t_token	*current;
	int	len;

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

bool squote_token(const char *cmd_line, size_t *i, t_token **token_list)
{
	t_token	*current;
	char	*token_value;
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

bool limiter_token(const char *cmd_line, size_t *i, t_token **token_list)
{
	t_token	*current;
	char	*token_value;
	t_index	index;
	bool	special;
	bool	error;
	
	token_value = NULL;
	special = 0;
	error = 0;
	index.j = *i;
	index.i = i;
	while (cmd_line[*index.i] && stop_limiter(cmd_line, index.i, 0) == 0)
	{
		if (cmd_line[index.j] == '\'')
		{
			special = 1;
			error = limiter_squote(cmd_line, &index, &token_value);
		}
		else if (cmd_line[index.j] == '\"')
			error = limiter_dquote(cmd_line, &index, &token_value);
		else
			error = limiter_word(cmd_line, &index, &token_value);
		if (error == 1)
		{
			if (token_value)
				free(token_value);
			return (1);
		}
	}
	if (!token_value)
	{
		if (token_addback(token_list, '\0', 2) == 1)
			return (1);
	}
	else
	{
		if (token_addback(token_list, token_value, 3) == 1)
			return (1);
	}
	current = token_last(*token_list);
	if (special == 1)
		current->type = WORD_SQLIMITER;
	else
		current->type = WORD_LIMITER;
	current->state = STATE_WORD;
	return (0);
}


bool	stop_limiter(const char *cmd_line, size_t *i, bool option)
{
	if (cmd_line[*i] == '&' || cmd_line[*i] == '|' || cmd_line[*i] == ' ' 
		|| cmd_line[*i] == '>' || cmd_line[*i] == '<' 
		|| cmd_line[*i] == '(' || cmd_line[*i] == ')')
		return (1);
	if (option == 1)
	{
		if (cmd_line[*i] == '\"' || cmd_line[*i] == '\'' )
			return (1);
	}
	return (0);
}

bool	limiter_squote(const char *cmd_line, t_index *index, char **token_value)
{
	char *temp;
	char *join;

	join = NULL;
	temp = NULL;
	(*index->i) = index_foward(&index->j); //j++ && i++
	if (!cmd_line[index->j])
		return (1);
	if (cmd_line[index->j] == '\'') //Apres dernier '
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
		temp = ft_substr(cmd_line, (*index->i), (index->j - *index->i));
		if (!temp)
			return (1);
		join = ft_strjoin(*token_value, temp);
		if (!join)
		{
			free(temp);
			return (1);
		}
		free(*token_value);
		free(temp);
		*token_value = join;
	}
	(*index->i) = index_foward(&index->j);
	return (0);
}

//Hello"
bool	limiter_dquote(const char *cmd_line, t_index *index, char **token_value)
{
	char *temp;
	char *join;

	join = NULL;
	temp = NULL;
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
		temp = ft_substr(cmd_line, (*index->i), (index->j - *index->i));
		if (!temp)
			return (1);
		join = ft_strjoin(*token_value, temp);
		if (!join)
		{
			free(temp);
			return (1);
		}
		free(*token_value);
		free(temp);
		*token_value = join;
	}
	(*index->i) = index_foward(&index->j);
	return (0);
}

bool	limiter_word(const char *cmd_line, t_index *index, char **token_value)
{
	char *temp;
	char *join;

	join = NULL;
	temp = NULL;
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
		temp = ft_substr(cmd_line, (*index->i), (index->j - *index->i));
		if (!temp)
			return (1);
		join = ft_strjoin(*token_value, temp);
		if (!join)
		{
			free(temp);
			return (1);
		}
		free(*token_value);
		free(temp);
		*token_value = join;
	}
	(*index->i) = index->j;
	return (0);
}
