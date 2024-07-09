/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   limiter_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 19:48:26 by allan             #+#    #+#             */
/*   Updated: 2024/07/09 13:06:11 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	last_heredoc(t_token **token_list)
{
	t_token	*current;
	int		heredoc;

	heredoc = 0;
	current = *token_list;
	while (current)
	{
		if (current->type == TOKEN_HEREDOC)
			heredoc++;
		current = current->next;
	}
	if (heredoc == 0)
		return (0);
	current = *token_list;
	while (current && heredoc > 0)
	{
		if (current->type == TOKEN_HEREDOC)
			heredoc--;
		current = current->next;
	}
	if (!current || !current->next)
		return (1);
	while (current && current->type == TOKEN_WHITESPACE)
		current = current->next;
	if (!current)
		return (1);
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

bool	limiter_join(const char *cmd_line, t_index *index, char **token_value)
{
	char *temp;
	char *join;

	join = NULL;
	temp = NULL;
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
	return (0);
}
