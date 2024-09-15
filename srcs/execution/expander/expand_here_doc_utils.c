/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_here_doc_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 17:43:21 by Matprod           #+#    #+#             */
/*   Updated: 2024/09/15 15:25:07 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	while_copy_folder(char *line, char *src, int fd_src, int fd_dest)
{
	while (line)
	{
		line = get_next_line(fd_src);
		if (!line)
			break ;
		ft_putstr_fd(line, fd_dest);
		free(line);
	}
	close(fd_src);
	close(fd_dest);
	unlink(src);
}

void	copy_folder(char *src, char *dest)
{
	char	*line;
	int		fd_src;
	int		fd_dest;

	line = "not NULL";
	fd_src = open(src, O_RDONLY);
	if (fd_src == -1)
	{
		ft_putstr_fd("Erreur lors de l'ouverture du fichier src", 2);
		return ;
	}
	fd_dest = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_dest == -1)
	{
		ft_putstr_fd("Erreur lors de l'ouverture du fichier dest", 2);
		close(fd_src);
		return ;
	}
	while_copy_folder(line, src, fd_src, fd_dest);
}

char	*ft_strdup_spe(char *s)
{
	char			*dup_str;
	unsigned int	index;
	unsigned int	length;

	if (!s)
		return (NULL);
	length = ft_strlen(s);
	dup_str = malloc((length + 3) * sizeof(char));
	if (!dup_str)
		return (NULL);
	index = 0;
	while (index < length)
	{
		dup_str[index] = s[index];
		index ++;
	}
	dup_str[index] = '\0';
	free(s);
	return (dup_str);
}
