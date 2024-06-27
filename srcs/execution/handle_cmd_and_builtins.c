/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_and_builtins.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 19:17:38 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/27 21:49:54 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	len_ast(t_ast *ast)
{
	int		i;
	t_ast	*temp;

	i = 1;
	temp = ast;
	while (temp && temp->left)
	{
		i++;
		temp = temp->left;
	}
	return (i);
}
char	**free_in_parent_cmd(char **cmd)
{
	free_array(cmd);
	return (NULL);
}

char	**parent_cmd(t_ast *current)
{
	char	**cmd;
	int		i;
	t_ast	*temp;

	cmd = (char **)malloc(len_ast(current) * sizeof(char *));
	if (!cmd)
		return (NULL);
	i = 1;
	temp = current;
	cmd[0] = ft_strdup(temp->value);
	if (!cmd)
		return(free_in_parent_cmd(cmd));
	while (temp && temp->left)
	{
		temp = temp->left;
		cmd[i] = ft_strdup(temp->value);
		if (!cmd)
			return(free_in_parent_cmd(cmd));
		i++;
	}
	cmd[i] = NULL;
	return (cmd);
}
