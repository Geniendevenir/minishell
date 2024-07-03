/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_operator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 13:22:53 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/02 19:02:39 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* int	exec_operator(t_ast *current, t_env *env)
{
	if (current->type == TOKEN_AND)
	{
		return (exec_and(current, env));
	}
	else if(current->type == TOKEN_OR)
	{
		return (exec_or(current, env));
	}
	return (-1);
}

int exec_and(t_ast *current, t_env *env)
{
	if (current->left->exit_state == 0)
		//return(exec_parent_node(current->right, env));
		return (1);
	else
		return (current->left->exit_state);
}

int exec_or(t_ast *current, t_env *env)
{
	if (current->left->exit_state == 0)
		return (current->left->exit_state);
	else
		//return (exec_parent_node(current->right, env));
		return (1);
} */