/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_word_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 19:24:26 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/16 15:24:03 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_word_part_cmd(char *word, t_word *boolean, t_env *env)
{
	if (check_builtin(word) == 1)
	{
		boolean->cmd = 1;
		return (WORD_BUILTIN);
	}
	else if (check_absolute_path_cmd(word) == 1)
	{
		boolean->cmd = 1;
		return (WORD_ABSPATH);
	}
	else if (check_cmd_exist(word, env) == 1)
	{
		boolean->cmd = 1;
		return (WORD_CMD);
	}
	else
		return (WORD_ERROR);
}

int	check_word_part_rediout(t_word *boolean)
{
	boolean->redi_out = 0;
	return (WORD_FILEOUT);
}

int	check_word_part_append(t_word *boolean)
{
	boolean->redi_out = 0;
	return (WORD_FILEOUT_APPEND);
}
