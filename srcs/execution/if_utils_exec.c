/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   if_utils_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 13:19:54 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/28 13:20:14 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool is_command_or_builtin_or_abspath(t_ast *current)
{
	if (current->type == WORD_CMD || current->type == WORD_ABSPATH 
		|| current->type == WORD_BUILTIN)
		return (1);
	else
		return (0);
}

bool is_redirect_folder(t_ast *current)
{
	if (current->type == WORD_FILEOUT_APPEND || current->type == WORD_FILEOUT 
		|| current->type == WORD_FILEIN || current->type == WORD_LIMITER)
		return (1);
	else
		return (0);
}