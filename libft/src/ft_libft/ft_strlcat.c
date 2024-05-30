/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 12:07:06 by adebert           #+#    #+#             */
/*   Updated: 2023/11/28 11:52:58 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size);

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dstlen;
	size_t	srclen;
	size_t	i;
	size_t	j;

	dstlen = ft_strlen(dst);
	srclen = ft_strlen(src);
	if ((size - 1 <= dstlen && (size + srclen) <= (srclen + dstlen))
		|| size == 0)
		return (size + srclen);
	i = dstlen;
	j = 0;
	while (src[j])
	{
		if (j < (size - dstlen - 1))
		{
			dst[i] = src[j];
			i++;
		}
		j++;
	}
	dst[i] = '\0';
	return (dstlen + srclen);
}
