/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 16:29:41 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/11 12:16:05 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	warning(char *str, int nb)
{
	int	i;

	i = 0;
	printf(
		"warning: here-document at line %d delimited by end-of-file (wanted `",
		nb);
	if (ft_strlen(str) == 0 || (ft_strlen(str) == 1 && str[0] == '\n'))
		printf("')\n");
	else
	{
		while ((size_t)i < ft_strlen(str) - 1)
			printf("%c", str[i++]);
		printf("')\n");
	}
}

int	quit_here_doc(int opt, t_all *p, int nb)
{
	if (opt == 0)
	{
		free(p->here_doc[nb]);
		p->here_doc[nb] = NULL;
	}
	else if (opt == 1)
	{
		free(p->here_doc[nb]);
		p->here_doc[nb] = NULL;
	}
	return (-1);
}

void	free_here_docs(char **here_docs)
{
	int	i;

	i = 0;
	if (here_docs == NULL)
		return ;
	while (here_docs[i] != NULL)
	{
		unlink(here_docs[i]);
		free(here_docs[i]);
		i++;
	}
	free(here_docs);
}

int	here_doc_count(t_token *tok)
{
	int	count;

	count = 0;
	while (tok)
	{
		if (tok->type == WORD_LIMITER || tok->type == WORD_SQLIMITER)
			count++;
		if (tok->next)
			tok = tok->next;
		else
			break ;
	}
	return (count);
}

void	init_here_docs(t_token *token_list, t_all **p)
{
	int		i;
	int		count;

	i = 0;
	count = here_doc_count(token_list);
	if (count == 0)
	{
		(*p)->here_doc = NULL;
		return ;
	}
	(*p)->here_doc = malloc((here_doc_count(token_list) + 1) * sizeof(char *));
	if (!(*p)->here_doc)
		return ;
	while (i <= count)
	{
		(*p)->here_doc[i] = NULL;
		i++;
	}
}

/* char	*ft_strjoin_spe(char *s1, char const *s2)
{
	char	*dest;
	size_t	i;
	size_t	destlen;

	i = 0;
	destlen = ft_strlen(s1) + ft_strlen(s2);
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
} */