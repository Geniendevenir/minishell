/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:31:18 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/01 17:15:17 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*ft_strndup(char *str, int n)
{
	char	*dup;
	int		i;

	i = 0;
	dup = malloc(sizeof(char) * (n + 1));
	if (!dup)
		return (NULL);
	while (i < n)
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

int	ft_strcmp(char *s1, char *s2)
{
	while (s1 && (*s1 != '\0' || *s2 != '\0'))
	{
		if (*s1 != *s2)
			return (*s1 - *s2);
		s1++;
		s2++;
	}
	return (0);
}
