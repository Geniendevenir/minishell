/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 16:47:23 by allan             #+#    #+#             */
/*   Updated: 2024/07/09 23:13:34 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//CHECK ERROR ENTRE RELINK ET WILDCARD
bool	expander(t_token **token_list, t_env *env, int error)
{
	t_token	*current;

	if (!env)
		remove_all_env(token_list);
	else if (expand_env(token_list, &env) == 1)
	{
		error_lexer(1);
		token_free(token_list);
		return (1);
	}
	error = 1;
	current = *token_list;
	if (relink_token(token_list, current, error) == 1)
	{
		error_lexer(1);
		token_free(token_list);
		return (1);
	}
	if (expand_wildcard(token_list, &error) == 1)
	{
		error_lexer(error);
		token_free(token_list);
		return (1);
	}
	return (0);
}


