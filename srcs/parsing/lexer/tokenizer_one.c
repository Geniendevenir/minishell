/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_one.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 17:26:32 by allan             #+#    #+#             */
/*   Updated: 2024/07/09 13:04:02 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	whitespace_token(const char *cmd_line, size_t *i, t_token **token_list)
{
	t_token	*current;
	
	if (token_addback(token_list, " ", 1) == 1)
		return (1);
	current = token_last(*token_list);
	current->len = 1;
	current->state = STATE_WHITESPACE;
	current->type = TOKEN_WHITESPACE;
	while (cmd_line[*i] && is_whitespace(cmd_line[*i]) == 1)
		(*i)++;
	return (0);
}

bool inpar_token(size_t *i, t_token **token_list)
{
	t_token	*current;
	
	if (token_addback(token_list, "(", 1) == 1)
		return (1);
	current = token_last(*token_list);
	current->len = 1;
	current->state = STATE_OPERATOR;
	current->type = TOKEN_OPENPAR;
	(*i)++;
	return (0);
}

bool outpar_token(size_t *i, t_token **token_list)
{
	t_token	*current;
	
	if (token_addback(token_list, ")", 1) == 1)
		return (1);
	current = token_last(*token_list);
	current->len = 1;
	current->state = STATE_OPERATOR;
	current->type = TOKEN_CLOSEPAR;
	(*i)++;
	return (0);
}

bool or_token(size_t *i, t_token **token_list)
{
	t_token	*current;
	
	if (token_addback(token_list, "||", 1) == 1)
		return (1);
	current = token_last(*token_list);
	current->len = 2;
	current->state = STATE_OPERATOR;
	current->type = TOKEN_OR;
	(*i) += 2;
	return (0);
}

bool pipe_token(size_t *i, t_token **token_list)
{
	t_token	*current;
	
	if (token_addback(token_list, "|", 1) == 1)
		return (1);
	current = token_last(*token_list);
	current->len = 1;
	current->state = STATE_OPERATOR;
	current->type = TOKEN_PIPE;
	(*i) += 1;
	return (0);
}
