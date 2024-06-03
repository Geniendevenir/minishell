/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_three.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 22:28:07 by allan             #+#    #+#             */
/*   Updated: 2024/06/03 21:56:59 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool outputapp_token(size_t *i, t_token **token_list)
{
	t_token	*current;
	
	token_addback(token_list, ">>");
	current = token_last(*token_list);
	if (!current)
		return (1);//add error
	current->len = 2;
	current->state = STATE_DONE;
	current->type = TOKEN_APPENDOUT;
	(*i) += 2;
	return (0);
}

bool outputre_token(size_t *i, t_token **token_list)
{
	t_token	*current;
	
	token_addback(token_list, ">");
	current = token_last(*token_list);
	if (!current)
		return (1);//add error
	current->len = 1;
	current->state = STATE_DONE;
	current->type = TOKEN_REDIRECTOUT;
	(*i)++;
	return (0);
}

bool	env_token(const char *cmd_line, size_t *i, t_token **token_list)
{
	t_token	*current;
	char	*token_value;
	size_t		j;
	
	j = *i + 1;
	if (cmd_line[j] == '?')
	{
		token_addback(token_list, "$?");
		j++;
	}
	else
	{
		while (cmd_line[j] && is_valid_env(cmd_line[j]))
			j++;
		token_value = ft_substr(cmd_line, *i, (j - *i)); //check valeur i et j;
		if (!token_value)
			return (1); //add error;
		token_addback(token_list, token_value);
	}
	current = token_last(*token_list);
	if (!current)
		return (1);//add error
	current->len = (j - *i);
	current->state = STATE_DONE;
	current->type = TOKEN_ENV;
	(*i) = j + 1;
	return (0);
}

bool lexical_token(const char *cmd_line, size_t *i, t_token **token_list)
{
	t_token	*current;
	char	*token_value;
	size_t	j;

	j = *i;
	printf("lexical j = %ld\n", j);
	while (cmd_line[j] && is_word(cmd_line[j]) == 0)
	{
		j++;
		printf("lexical j = %ld\n", j);
	}
	token_value = ft_substr(cmd_line, *i, (j - *i)); //check valeur i et j;
	if (!token_value)
		return (1); //add error;
	token_addback(token_list, token_value);
	current = token_last(*token_list);
	if (!current)
		return (1);//add error
	current->len = (j - *i);
	current->state = STATE_DONE;
	current->type = TOKEN_WORD;
	(*i) = j + 1;
	printf("lexical i = %ld\n", (*i));
	return (0);
}
