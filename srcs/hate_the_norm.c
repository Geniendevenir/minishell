/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hate_the_norm.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 19:33:11 by allan             #+#    #+#             */
/*   Updated: 2024/07/08 13:28:46 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	wildcard_return(DIR **d)
{
	closedir(*d);
	return (1);
}


void	index_init(t_index *index, size_t *i, int option)
{
	if (option == 1)
		index->j = *i;
	index->i = i;
	index->special = 0;
	index->error = 0;
}
