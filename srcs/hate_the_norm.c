/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hate_the_norm.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 19:33:11 by allan             #+#    #+#             */
/*   Updated: 2024/07/11 19:30:06 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	wildcard_return(DIR **d)
{
	closedir(*d);
	return (1);
}
bool	is_file(t_ast *current)
{
	if ((current)->type == WORD_FILEOUT || (current)->type == WORD_FILEIN 
		|| (current)->type == WORD_FILEOUT_APPEND)
		return (1);
	return (0);
}

void	index_init(t_index *index, size_t *i, int option)
{
	if (option == 1)
		index->j = *i;
	index->i = i;
	index->special = 0;
	index->error = 0;
}

size_t	index_foward(size_t *j)
{
	*j += 1;
	return (*j);
}
