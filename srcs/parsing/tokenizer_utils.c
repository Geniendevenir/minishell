/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:19:15 by allan             #+#    #+#             */
/*   Updated: 2024/06/03 18:28:30 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_whitespace(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

bool	is_env(char c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_'
		|| c == '?')
		return (0);
	return (1);
}

bool	is_valid_env(char c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_'
		|| (c >= '0' && c <= '9'))
		return (0);
	return (1);
}

bool	is_word(char c)
{
	if (c == '|' || c == '&' || c == '$' || c == '\'' || c == '\"'
		|| c == '<' || c == '>' || c == '(' || c == ')')
		return (1);
	if (is_whitespace(c) == 1)
		return (1);
	return (0);
}

bool	is_freeable(char *value)
{
	int	i;

	i = ft_strlen(value);
	if (ft_strncmp(value, " ", i) == 0 || ft_strncmp(value, ")", i)
		|| ft_strncmp(value, "(", i) || ft_strncmp(value, "<", i))
		return (1);
	if (ft_strncmp(value, "||", i) == 0 || ft_strncmp(value, "|", i)
		|| ft_strncmp(value, "&&", i) || ft_strncmp(value, "<<", i))
		return (1);
	if (ft_strncmp(value, ">>", i) == 0 || ft_strncmp(value, ">", i)
		|| ft_strncmp(value, "$?", i))
		return (1);
	return (0);
}