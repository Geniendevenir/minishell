/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 16:47:23 by allan             #+#    #+#             */
/*   Updated: 2024/07/11 18:46:47 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*CHECK ERROR ENTRE RELINK ET WILDCARD
L24 //add a exemption rule for $? that are still expanded
*/
bool	expander(t_token **token_list, t_all *p, int error)
{
	t_token	*current;

	if (!p->env)
		remove_all_env(token_list, p->exit_status);
	else if (expand_env(token_list, &p->env, p->exit_status) == 1)
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
