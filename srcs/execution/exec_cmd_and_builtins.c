/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_and_builtins.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 19:17:38 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/02 19:05:14 by Matprod          ###   ########.fr       */
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

/*return a char**cmd | cmd[0] = command name and cmd[n] = n options
else return NULL*/
char	**split_cmd(t_ast *current)
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

/* int exec_cmd_or_builtin(t_ast *current, t_all *env)
{
	char	**cmd; //sera a free plus tard;

	cmd = split_cmd(current);
	if (current->type == WORD_BUILTIN)
		//return(exec_builtins(cmd, env));
		return (1);
	else if (current->type == WORD_CMD || current->type == WORD_ABSPATH)
		//return(exec_cmd(cmd, env));
		return (1);
	return (-1);
	
} */ 