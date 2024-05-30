/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 11:26:27 by allan             #+#    #+#             */
/*   Updated: 2024/04/17 05:02:28 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

char	*get_current_line(int fd, char *stash)
{
	int		i;
	char	*buffer;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	i = 1;
	while (!gnl_strchr(stash, '\n') && i != 0)
	{
		i = read(fd, buffer, BUFFER_SIZE);
		if (i == -1)
		{
			free (buffer);
			free (stash);
			return (NULL);
		}
		buffer[i] = '\0';
		if (!stash)
			stash = gnl_strdup(buffer);
		else
			stash = gnl_strjoin(stash, buffer);
	}
	free(buffer);
	return (stash);
}

char	*stash_to_display(char *stash)
{
	size_t	i;
	char	*display;

	i = 0;
	if (!*stash)
		return (NULL);
	while (stash[i] && stash[i] != '\n')
		i++;
	display = malloc(sizeof(char) * (i + 2));
	if (!display)
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n' )
	{
		display[i] = stash[i];
		i++;
	}
	if (stash[i] == '\n')
	{
		display[i] = stash[i];
		i++;
	}
	display[i] = '\0';
	return (display);
}

char	*stash_reset(char *stash)
{
	size_t	i;
	size_t	j;
	char	*stash_reset;

	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\0')
	{
		free(stash);
		return (NULL);
	}
	i++;
	stash_reset = malloc(sizeof(char) * (gnl_strlen(stash) - i + 1));
	if (!stash_reset)
		return (NULL);
	j = 0;
	while (stash[i])
		stash_reset[j++] = stash[i++];
	stash_reset[j] = '\0';
	free(stash);
	return (stash_reset);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*display;

	if (fd < 0 && BUFFER_SIZE <= 0)
		return (NULL);
	stash = get_current_line(fd, stash);
	if (!stash)
		return (NULL);
	display = stash_to_display(stash);
	stash = stash_reset(stash);
	return (display);
}
