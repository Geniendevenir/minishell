/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_parenthesis_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 12:16:17 by allan             #+#    #+#             */
/*   Updated: 2024/09/16 11:55:58 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_parenthesis_error(t_token *current, t_syntax *syntax, int *skip)
{
	if (current->type == TOKEN_CLOSEPAR)
	{
		if (is_parenthesis_error(current, *syntax, 1) == 1)
			return (closepar_error(current, *syntax, skip));
		syntax->openpar--;
	}
	else if (current->type == TOKEN_OPENPAR)
	{
		if (is_parenthesis_error(current, *syntax, 2) == 1)
			return (openpar_error(current, *syntax, skip));
		syntax->openpar++;
	}
	return (0);
}
