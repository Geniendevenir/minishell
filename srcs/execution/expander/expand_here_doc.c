/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_here_doc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 12:18:13 by Matprod           #+#    #+#             */
/*   Updated: 2024/09/13 17:15:14 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define TAILLE_BUFFER 1024

void print_folder(char *fd_src, int fd)
{
	char *line;

	line = "not NULL";
	fd = open(fd_src, O_RDONLY);
	while (line)
	{
		line = get_next_line(fd);
		if (!line)
			break;
		ft_putstr(line);
		free(line);
	}
	close(fd);
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
		return;
	}	
	fd_dest = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_dest == -1)
	{
		ft_putstr_fd("Erreur lors de l'ouverture du fichier dest", 2);
		close(fd_src);
		return;
	}
	while (line)
	{
		line = get_next_line(fd_src);
		if (!line)
			break;
		ft_putstr_fd(line, fd_dest);
		free(line);
	}
	close(fd_src);
	close(fd_dest);
	print_folder(dest, fd_dest);
	unlink(src);
}

char	*ft_strdup_spe(char *s)
{
	char			*dup_str;
	unsigned int	index;
	unsigned int	length;

	if (!s)
		return (NULL);
	length = ft_strlen(s);
	dup_str = malloc((length + 3) * sizeof(char)); //a cehck
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

int get_nb_line(t_all *p)
{
	int		fd;
	int		nb;
	char	*line;

	nb = 0;
	line = "not NULL";
	fd = open(p->here_doc[p->int_here_doc], O_RDONLY);
	if (fd == -1)
		return (ft_putstr_fd("error openfd1\n", 2), 0);
	while (line != NULL)
	{
		line = get_next_line(fd);
		nb++;
		free(line);
	}
	close(fd);
	return (nb);
}

int get_token_list(t_all *p, t_token **token, char *line)
{
	size_t		i;
	int			error;

	error = 0;
	i = 0;
	printf("line = %s\n", line);
	while (i < ft_strlen(line))
	{
		error = split_one(line, &i, token, 1);
		if (error != 0)
		{
			token_free(token);
			error_lexer(error);
			return (1);
		}
		//i++;
	}
	token_print_amazing(token);
	if (expander(token, p, error) == 1) //free token list automatiquement
		return (1);
	
	return (0);
}

char *expand_line(t_all *p, char *line)
{
	t_token	*token;
	char	*new_line;

	new_line = NULL;
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token_init(&token);
	if (get_token_list(p, &token, line) == 1)
	{
		write(2, "ERROR GET TOKEN LIST\n", 21);
		return (NULL);
	}
	new_line = ft_strdup(token->value);
	if (token->next)
		token = token->next;
	while (token->next)
	{
		new_line = ft_strjoin_spe(new_line, token->value);
		//new_line = ft_strjoin_spe(new_line, "\n");
		token = token->next;
	}
	free(line);
	token_free(&token);
	printf("new_line = %s\n",new_line);
	return (new_line);
}


void expand_heredoc(t_all *p)
{
	int		fd;
	int		fd2;
	char	*line;

	line = "not NULL";
	if (!p->here_doc || !p->here_doc[p->int_here_doc])
		return;
	fd = open(p->here_doc[p->int_here_doc], O_RDONLY);
	if (fd == -1)
		return (ft_putstr_fd("error openfd1\n", 2));
	fd2 = open("fd2", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd2 == -1)
	{
		close(fd);
		close(fd2);
		return (ft_putstr_fd("error open fd2\n", 2));
	}
	while (line)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break;
		printf("line before expand line = %s\n", line);
		line = ft_strdup_spe(expand_line(p, line));
		printf("line after expand line = %s\n", line);
		write(fd2, line, ft_strlen(line));
		write(fd2, "\n", 1);
		free(line);
	}
	close(fd);
	close(fd2);
	copy_folder("fd2" , p->here_doc[p->int_here_doc]);
	p->int_here_doc++;
}
