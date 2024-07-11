/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_two_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 18:26:14 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/11 18:45:52 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
L23  //substr de i + 1 index->jusqu'a index->j
*/
bool	in_if_envdquotes(char *tokenv, const char *cl, t_index *i, t_token **tl)
{
	tokenv = ft_substr(cl, *i->i, (i->j - *i->i));
	if (!tokenv)
		return (1);
	if (dquote_add_token(tokenv, tl, 0) == 1)
		return (1);
	return (0);
}

bool	end_of_env_dquotes(char *token_value, t_token **token_list, int option)
{
	if (!token_value)
		return (1);
	if (dquote_add_token(token_value, token_list, option) == 1)
		return (1);
	return (0);
}
