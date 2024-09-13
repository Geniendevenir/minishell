/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_here_doc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 12:18:13 by Matprod           #+#    #+#             */
/*   Updated: 2024/09/13 17:43:53 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	{
		close(fd);
		return (ft_putstr_fd("error openfd1\n", 2));
	}
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
		line = ft_strdup_spe(expand_line(p, line));
		write(fd2, line, ft_strlen(line));
		write(fd2, "\n", 1);
		free(line);
	}
	close(fd);
	close(fd2);
	copy_folder("fd2" , p->here_doc[p->int_here_doc]);
	p->int_here_doc++;
}
