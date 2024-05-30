/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 11:26:30 by allan             #+#    #+#             */
/*   Updated: 2024/04/17 13:52:42 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	gnl_strchr(const char *s, int c)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		if (s[i] == c)
			return (1);
		i++;
	}
	return (0);
}

size_t	gnl_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*gnl_strjoin(char *s1, char const *s2)
{
	char	*dest;
	size_t	i;
	size_t	destlen;

	i = 0;
	destlen = gnl_strlen(s1) + gnl_strlen(s2);
	dest = malloc(destlen + 1 * sizeof(char));
	if (!dest)
		return (NULL);
	while (*(s1 + i))
	{
		*(dest + i) = *(s1 + i);
		i++;
	}
	while (*s2)
	{
		*(dest + i) = *s2++;
		i++;
	}
	*(dest + i) = '\0';
	free(s1);
	return (dest);
}

char	*gnl_strdup(char *s)
{
	size_t	slen;
	char	*scopy;

	slen = gnl_strlen(s);
	scopy = malloc((slen + 1) * sizeof(char));
	if (!scopy)
		return (NULL);
	while (*s)
		*scopy++ = *s++;
	*scopy = '\0';
	scopy = scopy - ((slen) * sizeof(char));
	return (scopy);
}
