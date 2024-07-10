/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_three.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 22:28:07 by allan             #+#    #+#             */
/*   Updated: 2024/07/09 13:16:32 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool lexical_token(const char *cmd_line, size_t *i, t_token **token_list)
{
	t_token	*current;
	char	*token_value;
	size_t	j;

	j = *i;
	while (cmd_line[j])
	{
		if (cmd_line[j] == '\'')
			j = skip_quotes(cmd_line, j, 1);
		else if (cmd_line[j] == '\"')
			j = skip_quotes(cmd_line, j, 0);
		else if (is_word(cmd_line[j]) == 1)
			break;
		j++;
	}
	token_value = ft_substr(cmd_line, *i, (j - *i));
	if (!token_value)
		return (1);
	if (token_addback(token_list, token_value, 0) == 1)
		return (1);
	current = token_last(*token_list);
	current->state = STATE_WORD;
	current->type = TOKEN_WORD;
	(*i) = j;
	return (0);
}
