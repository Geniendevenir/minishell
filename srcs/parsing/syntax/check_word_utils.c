/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_word_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 19:24:26 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/09 19:13:35 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_word_part_cmd(char *word, t_word *boolean)
{
	if (check_builtin(word) == 1)
	{
		boolean->cmd = 1;
		return (WORD_CMD);
	}
	else
	{
		boolean->cmd = 1;
		return (WORD_CMD);
	}
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
