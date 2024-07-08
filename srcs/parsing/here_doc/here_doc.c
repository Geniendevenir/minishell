/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 14:57:22 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/08 23:15:16 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*cleanbuffer(char *buffer)
{
	if (buffer != NULL)
	{
		free(buffer);
		buffer = NULL;
	}
	return (buffer);
}

int	prev_valo(char *buffer)
{
	if (buffer != NULL && ft_strlen(buffer) >= 1
		&& buffer[ft_strlen(buffer) - 1] == '\n')
		return (1);
	return (0);
}

char	*hdoc_process(int fd, t_token *limiter, t_all **p)
{
	extern int	sig_int;
	char		*buffer;
	int			prev;

	buffer = get_next_line(0);
	if (limiter->value)
		limiter->value = ft_strjoin(limiter->value, "\n");
	else if (!*limiter->value)
		limiter->value = ft_strdup("\n");
	if (buffer == NULL && sig_int != 1)
		return (warning(limiter->value, (*p)->line_num), NULL);
	prev = prev_valo(buffer);
	(*p)->line_num = 1;
		
	while (ft_strcmp(buffer, limiter->value) != 0 && sig_int == 0)
	{
		printf("buffer = |%s| | limiter = |%s|\n",buffer, limiter->value);
		if (buffer != NULL)
			write(fd, buffer, ft_strlen(buffer));
		if (buffer != NULL)
			free(buffer);
		buffer = (get_next_line(0));
		(*p)->line_num++;
		if (prev == 1 && buffer == NULL && sig_int != 1)
		{
			warning(limiter->value, (*p)->line_num);
			break ;
		}
		prev = prev_valo(buffer);
	}
	return (cleanbuffer(buffer));
}

int	fill_here_doc(t_token **current, int max, t_all **p, int *nb)
{
	extern int sig_int;
	int		fd;
	char	*buffer;

	printf("TEST 1\n");
	if (*nb == max)
		return ((*p)->here_doc[*nb] = NULL, 1);
	printf("TEST 2\n");
	(*p)->here_doc[*nb] = generate_name();
	fd = open((*p)->here_doc[*nb], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd == -1)
		return (free((*p)->here_doc[*nb]), (*p)->here_doc[*nb] = NULL, -1);
	if (signals_hdoc(0, p) == -1)
		return (quit_here_doc(0, *p, *nb));
	if ((*current)->type == WORD_SQLIMITER)
		buffer = hdoc_process(fd, *current, p);
	/*else if ((*current)->type == WORD_LIMITER)
		buffer = hdoc_process(fd, *current); */
	if (*nb + 1 == max && (buffer || !buffer))
		buffer = get_next_line(-42);
	(*current)->type = WORD_FILEIN;
	free((*current)->value);
	(*current)->value = ft_strdup((*p)->here_doc[*nb]);
	(*nb)++;
	if (/* signals_hdoc(1, p) == -1 ||  */(*current)->value == NULL || sig_int == 1)
		return (close (fd), quit_here_doc(1, *p, *nb));
	else
		return (close (fd), 1);
}

void here_doc(t_token **token_list, t_all **p)
{
	t_token *current;
	int		max;
	int		nb;

	nb = 0;	
	max = here_doc_count(*token_list);
	current = *token_list;
	init_here_docs(*token_list, p);
	while (current)
	{
		if (current->type == WORD_LIMITER || current->type == WORD_SQLIMITER)
		{
			if (fill_here_doc(&current, max, p, &nb) == -1)
			{
				free_here_docs((*p)->here_doc);
				(*p)->here_doc = NULL;
			}
		}
		if (current->next)
			current = current->next;
		else
			break;
	}
}