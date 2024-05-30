/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 23:10:46 by allan             #+#    #+#             */
/*   Updated: 2023/11/29 18:27:23 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c);

char	*ft_strchr(const char *s, int c)
{
	size_t	i;

	i = 0;
	while (i < ft_strlen(s))
	{
		if (*((char *)s + i) == (unsigned char)c)
			return (((char *)s + i));
		i++;
	}
	if (*((char *)s + i) == (unsigned char)c)
		return (((char *)s + i));
	return (NULL);
}
