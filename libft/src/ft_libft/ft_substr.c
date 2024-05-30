/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 18:23:21 by allan             #+#    #+#             */
/*   Updated: 2023/12/02 23:23:04 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *source, unsigned int start, size_t len);

char	*ft_substr(char const *source, unsigned int start, size_t len)
{
	char	*dest;
	size_t	i;

	if (start > ft_strlen(source))
		len = 0;
	else if (len > ft_strlen(source + start))
		len = ft_strlen(source + start);
	dest = malloc((len + 1) * sizeof(char));
	if (!dest)
		return (NULL);
	i = 0;
	while (start + i < ft_strlen(source) && i < len)
	{
		dest[i] = source[start + i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
