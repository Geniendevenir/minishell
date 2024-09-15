/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:31:18 by Matprod           #+#    #+#             */
/*   Updated: 2024/09/15 12:52:35 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*ft_strndup(char *str, int n)
{
	char	*dup;
	int		i;

	i = 0;
	dup = malloc(sizeof(char) * (n + 1));
	if (!dup)
		return (NULL);
	while (i < n)
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

int	ft_strcmp(char *s1, char *s2)
{
	while (s1 && (*s1 != '\0' || *s2 != '\0'))
	{
		if (*s1 != *s2)
			return (*s1 - *s2);
		s1++;
		s2++;
	}
	return (0);
}

char	*ft_strjoin_spe(char *s1, char const *s2)
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
}

void	trim_space(char *str)
{
	char	*debut;
	char	*fin;

	debut = str;
	while (is_whitespace((unsigned char)*debut))
		debut++;
	if (*debut == '\0')
	{
		str[0] = '\0';
		return ;
	}
	fin = debut + ft_strlen(debut) - 1;
	while (fin > debut && isspace((unsigned char)*fin))
		fin--;
	*(fin + 1) = '\0';
	ft_memmove(str, debut, fin - debut + 2);
}

void update_variable(t_all *p)
{
	extern int g_sig_int;
	
	g_sig_int = 0;
	free(p->line);
	p->line = NULL;
	p->int_here_doc = 0;
	p->line_num++;
}