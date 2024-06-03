/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_one.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 17:26:32 by allan             #+#    #+#             */
/*   Updated: 2024/06/03 21:56:13 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	whitespace_token(const char *cmd_line, size_t *i, t_token **token_list)
{
	t_token	*current;
	
	token_addback(token_list, " ");
	current = token_last(*token_list);
	if (!current)
		return (1);//add error
	current->len = 1;
	current->state = STATE_DONE;
	current->type = TOKEN_WHITESPACE;
	while (cmd_line[*i] && ((cmd_line[*i] >= 9 && cmd_line[*i] <= 13)
		|| cmd_line[*i] == 32))
		(*i)++;
	return (0);
}

bool dquote_token(const char *cmd_line, size_t *i, t_token **token_list)
{
	t_token	*current;
	char	*token_value;
	size_t		j;

	j = *i + 1;
	if (cmd_line[j] == '\"')
	{
		(*i) += 1;
		return (1);
	}
	while (cmd_line[j] && cmd_line[j] != '\"')
		j++;
	token_value = ft_substr(cmd_line, (*i) + 1, (j - *i)); //check valeur i et j;
	if (!token_value)
		return (1); //add error;
	token_addback(token_list, token_value);
	current = token_last(*token_list);
	if (!current)
		return (1);//add error
	current->len = (j - (*i));
	current->state = STATE_DONE;
	current->type = TOKEN_DQUOTES;
	(*i) = j + 1;
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
		(*i) += 1;
		return (1);
	}
	while (cmd_line[j] && cmd_line[j] != '\'')
		j++;
	token_value = ft_substr(cmd_line, (*i) + 1, (j - *i)); //check valeur i et j;
	if (!token_value)
		return (1); //add error;
	token_addback(token_list, token_value);
	current = token_last(*token_list);
	if (!current)
		return (1);//add error
	current->len = (j - *i);
	current->state = STATE_DONE;
	current->type = TOKEN_SQUOTES;
	(*i) = j + 1;
	return (0);
}

bool inpar_token(size_t *i, t_token **token_list)
{
	t_token	*current;
	
	token_addback(token_list, "(");
	current = token_last(*token_list);
	if (!current)
		return (1);//add error
	current->len = 1;
	current->state = STATE_DONE;
	current->type = TOKEN_OPENPAR;
	(*i)++;
	return (0);
}

bool outpar_token(size_t *i, t_token **token_list)
{
	t_token	*current;
	
	token_addback(token_list, ")");
	current = token_last(*token_list);
	if (!current)
		return (1); //add error
	current->len = 1;
	current->state = STATE_DONE;
	current->type = TOKEN_CLOSEPAR;
	(*i)++;
	return (0);
}
