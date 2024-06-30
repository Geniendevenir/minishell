/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_operator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 13:22:53 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/30 19:14:08 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_operator(t_ast *current, t_env *env)
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
		return(exec_parent_node(current->right, env));
	else
		return (current->left->exit_state);
}

int exec_or(t_ast *current, t_env *env)
{
	if (current->left->exit_state == 0)
		return (current->left->exit_state);
	else
		return (exec_parent_node(current->right, env));
}