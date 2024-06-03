/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_two.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 17:26:32 by allan             #+#    #+#             */
/*   Updated: 2024/06/03 21:56:38 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool or_token(size_t *i, t_token **token_list)
{
	t_token	*current;
	
	token_addback(token_list, "||");
	current = token_last(*token_list);
	if (!current)
		return (1);//add error
	current->len = 2;
	current->state = STATE_DONE;
	current->type = TOKEN_OR;
	(*i) += 2;
	return (0);
}
bool pipe_token(size_t *i, t_token **token_list)
{
	t_token	*current;
	
	token_addback(token_list, "|");
	current = token_last(*token_list);
	if (!current)
		return (1);//add error
	current->len = 1;
	current->state = STATE_DONE;
	current->type = TOKEN_PIPE;
	(*i) += 1;
	return (0);
}
bool and_token(size_t *i, t_token **token_list)
{
	t_token	*current;
	
	token_addback(token_list, "&&");
	current = token_last(*token_list);
	if (!current)
		return (1);//add error
	current->len = 2;
	current->state = STATE_DONE;
	current->type = TOKEN_AND;
	(*i) += 2;
	return (0);
}
bool heredoc_token(size_t *i, t_token **token_list)
{
	t_token	*current;
	
	token_addback(token_list, "<<");
	current = token_last(*token_list);
	if (!current)
		return (1);//add error
	current->len = 2;
	current->state = STATE_DONE;
	current->type = TOKEN_HEREDOC;
	(*i) += 2;
	return (0);
}
bool inputre_token(size_t *i, t_token **token_list)
{
	t_token	*current;
	
	token_addback(token_list, "<");
	current = token_last(*token_list);
	if (!current)
		return (1);//add error
	current->len = 1;
	current->state = STATE_DONE;
	current->type = TOKEN_REDIRECTIN;
	(*i)++;
	return (0);
}