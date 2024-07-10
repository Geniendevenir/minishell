/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_two.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 17:26:32 by allan             #+#    #+#             */
/*   Updated: 2024/07/02 15:06:46 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int and_token(const char *cmd_line, size_t *i, t_token **token_list)
{
	t_token	*current;
	
	if (cmd_line[*i + 1] != '&')
	{
		if (token_addback(token_list, "&", 2) == 1)
			return (1);
		(*i) += 1;
	}
	else
	{
		if (token_addback(token_list, "&&", 2) == 1)
			return (1);
		(*i) += 2;
	}
	current = token_last(*token_list);
	current->len = 2;
	current->state = STATE_OPERATOR;
	current->type = TOKEN_AND;
	return (0);
}

bool heredoc_token(size_t *i, t_token **token_list)
{
	t_token	*current;
	
	if (token_addback(token_list, "<<", 1) == 1)
		return (1);
	current = token_last(*token_list);
	current->len = 2;
	current->state = STATE_OPERATOR;
	current->type = TOKEN_HEREDOC;
	(*i) += 2;
	return (0);
}

bool inputre_token(size_t *i, t_token **token_list)
{
	t_token	*current;
	
	if (token_addback(token_list, "<", 1) == 1)
		return (1);
	current = token_last(*token_list);
	current->len = 1;
	current->state = STATE_OPERATOR;
	current->type = TOKEN_REDIRECTIN;
	(*i)++;
	return (0);
}

bool outputapp_token(size_t *i, t_token **token_list)
{
	t_token	*current;
	
	if (token_addback(token_list, ">>", 1) == 1)
		return (1);
	current = token_last(*token_list);
	current->len = 2;
	current->state = STATE_OPERATOR;
	current->type = TOKEN_APPENDOUT;
	(*i) += 2;
	return (0);
}

bool outputre_token(size_t *i, t_token **token_list)
{
	t_token	*current;
	
	if (token_addback(token_list, ">", 1) == 1)
		return (1);
	current = token_last(*token_list);
	current->len = 1;
	current->state = STATE_OPERATOR;
	current->type = TOKEN_REDIRECTOUT;
	(*i)++;
	return (0);
}
