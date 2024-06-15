/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_three.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 22:28:07 by allan             #+#    #+#             */
/*   Updated: 2024/06/15 17:15:33 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_token(const char *cmd_line, size_t *i, t_token **token_list)
{
	t_token	*current;
	char	*token_value;
	size_t		j;
	
	j = *i;
	if (is_env(cmd_line[j + 1]) == 1)
		return (3);
	else
	{
		while (cmd_line[j + 1] && (is_valid_env(cmd_line[j + 1]) == 0))
			j++;
		token_value = ft_substr(cmd_line, (*i) + 1, (j - *i));
		if (!token_value)
			return (1); //add error;
		if (token_addback(token_list, token_value, 0) == 1)
			return (1);
	}
	current = token_last(*token_list);
	if (!current)
		return (1);//add error
	current->len = (j - *i) + 1;
	current->state = STATE_WORD;
	current->type = TOKEN_ENV;
	(*i) = j + 1;
	return (0);
}

bool	wildcard_token(const char *cmd_line, size_t *i, t_token **token_list)
{
	t_token	*current;
	char	*token_value;
	size_t	j;

	j = *i;
	while (cmd_line[j] && (is_word(cmd_line[j]) == 0 || cmd_line[j] == '*'))
		j++;
	token_value = ft_substr(cmd_line, *i, (j - *i));
	if (!token_value)
		return (1);
	if (token_addback(token_list, token_value, 0) == 1)
		return (1);
	current = token_last(*token_list);
	if (!current)
		return (1);
	current->len = (j - *i);
	current->state = STATE_WORD;
	current->type = TOKEN_WILDCARD;
	(*i) = j;
	return (0);
}

bool lexical_token(const char *cmd_line, size_t *i, t_token **token_list)
{
	t_token	*current;
	char	*token_value;
	size_t	j;

	j = *i;
	while (cmd_line[j] && is_word(cmd_line[j]) == 0)
		j++;
	token_value = ft_substr(cmd_line, *i, (j - *i)); //check valeur i et j;
	if (!token_value)
		return (1); //add error;
	if (token_addback(token_list, token_value, 0) == 1)
		return (1);
	current = token_last(*token_list);
	if (!current)
		return (1);//add error
	current->len = (j - *i);
	current->state = STATE_WORD;
	current->type = TOKEN_WORD;
	(*i) = j;
	return (0);
}
