/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hate_the_norm.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 19:33:11 by allan             #+#    #+#             */
/*   Updated: 2024/07/01 19:34:23 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	wildcard_return(DIR **d)
{
	closedir(*d);
	return (1);
}
