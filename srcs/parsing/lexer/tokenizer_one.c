/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_one.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 17:26:32 by allan             #+#    #+#             */
/*   Updated: 2024/06/09 00:25:02 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	whitespace_token(const char *cmd_line, size_t *i, t_token **token_list)
{
	t_token	*current;
	
	if (token_addback(token_list, " ") == 1)
		return (1);
	current = token_last(*token_list);
	if (!current)
		return (1);//add error
	current->len = 1;
	current->state = STATE_WHITESPACE;
	current->type = TOKEN_WHITESPACE;
	while (cmd_line[*i] && is_whitespace(cmd_line[*i]) == 1)
		(*i)++;
	return (0);
}

/* bool dquote_token(const char *cmd_line, size_t *i, t_token **token_list)
{
	t_token	*current;
	char	*token_value;
	size_t		j;

	j = *i + 1;
	if (cmd_line[j] == '\"')
	{
		(*i) += 2;
		return (0);
	}
	while (cmd_line[j] && cmd_line[j] != '\"')
		j++;
	token_value = ft_substr(cmd_line, (*i) + 1, (j - *i) - 1); //check valeur i et j;
	if (!token_value)
		return (1); //add error;
	if (token_addback(token_list, token_value) == 1)
		return (1);
	current = token_last(*token_list);
	if (!current)
		return (1);//add error
	current->len = (j - (*i));
	current->state = STATE_WORD;
	current->type = TOKEN_DQUOTES;
	(*i) = j + 1;
	return (0);
} */

bool	env_dquotes_token(const char *cmd_line, size_t *i, t_token **token_list) //RECODER ET REGLER PROBLEME "$?"
{
	char	*token_value;
	size_t		j;

	if (cmd_line[*i + 1] == '\"')
	{
		(*i) += 2;
		return (0);
	}
	(*i)++; //1
	j = *i; //1
	while (cmd_line[j] && cmd_line[j] != '\"')
	{
		if (cmd_line[j] == '$' && is_env(cmd_line[j + 1]) == 0)
		{
			if (j > *i)
			{
				token_value = ft_substr(cmd_line, *i, (j - *i));
				if (!token_value)
					return (1);
				dquote_add_token(token_value, token_list, 0); //substr de i + 1 jusqu'a j
			}
			j++;
			(*i) = j;
			while (is_valid_env(cmd_line[j]) == 0)
				j++;
			token_value = ft_substr(cmd_line, *i, (j - *i));
			if (!token_value)
				return (1);
			dquote_add_token(token_value, token_list, 1);
			//j++;
			(*i) = j;	
		}
		if (cmd_line[*i] == '$' && is_env(cmd_line[*i + 1]) == 0)
			(*i)++;
		else
			j++;
	}
	if (j > *i && cmd_line[*i] != '"')
	{
		token_value = ft_substr(cmd_line, *i, (j - *i));
		if (!token_value)
			return (1);
		dquote_add_token(token_value, token_list, 0);
	}
	(*i) = j + 1;
	return (0);
}

bool	dquote_add_token(char *token_value, t_token **token_list, bool option)
{
	t_token	*current;
	int	len;

	len = ft_strlen(token_value);
	if (token_addback(token_list, token_value) == 1)
		return (1);
	current = token_last(*token_list);
	if (!current)
		return (1);//add error
	current->len = len;
	current->state = STATE_WORD;
	if (option == 1)
		current->type = TOKEN_ENV;
	else if (option == 0)
		current->type = TOKEN_DQUOTES;
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
	token_value = ft_substr(cmd_line, (*i) + 1, (j - *i) - 1); //check valeur i et j;
	if (!token_value)
		return (1);
	if (token_addback(token_list, token_value) == 1)
		return (1);
	current = token_last(*token_list);
	if (!current)
		return (1);//add error
	current->len = (j - *i);
	current->state = STATE_WORD;
	current->type = TOKEN_SQUOTES;
	(*i) = j + 1;
	return (0);
}

bool inpar_token(size_t *i, t_token **token_list)
{
	t_token	*current;
	
	if (token_addback(token_list, "(") == 1)
		return (1);
	current = token_last(*token_list);
	if (!current)
		return (1);//add error
	current->len = 1;
	current->state = STATE_OPERATOR;
	current->type = TOKEN_OPENPAR;
	(*i)++;
	return (0);
}

bool outpar_token(size_t *i, t_token **token_list)
{
	t_token	*current;
	
	if (token_addback(token_list, ")") == 1)
		return (1);
	current = token_last(*token_list);
	if (!current)
		return (1);
	current->len = 1;
	current->state = STATE_OPERATOR;
	current->type = TOKEN_CLOSEPAR;
	(*i)++;
	return (0);
}
