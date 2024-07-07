/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hate_the_norm.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 19:33:11 by allan             #+#    #+#             */
/*   Updated: 2024/07/07 19:49:41 by allan            ###   ########.fr       */
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
