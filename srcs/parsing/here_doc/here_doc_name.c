/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_name.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 16:31:00 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/11 12:40:51 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	fichier_existe(const char *name)
{
	return (access(name, F_OK) != -1);
}

void	if_in_increment_base(size_t len, size_t *j, char *name)
{
	if (!name)
		return ;
	len = ft_strlen(name);
	*j = len;
	while (*j > 0)
	{
		name[*j] = name[*j - 1];
		(*j)--;
	}
	name[0] = BASE[0];
	name[len + 1] = '\0';
}

static void	init_i_j_len_(size_t *i, size_t *j, size_t *len, char *name)
{
	*j = 0;
	*len = 0;
	*i = ft_strlen(name) - 1;
}

void	increment_base(char *name, size_t len_base)
{
	size_t	i;
	size_t	j;
	size_t	len;
	char	*pos;

	init_i_j_len_(&i, &j, &len, name);
	while (i != (size_t)-1)
	{
		pos = ft_strchr(BASE, name[i]);
		if (pos && *pos != BASE[len_base - 1])
		{
			name[i] = BASE[(pos - BASE + 1) % len_base];
			return ;
		}
		else
		{
			name[i] = BASE[0];
			if (i-- == 0)
			{
				if (name)
					if_in_increment_base(len, &j, name);
				return ;
			}
		}
	}
}

char	*generate_name(void)
{
	char	*name;

	name = malloc(MAX_FILENAME_LENGTH + 1);
	if (!name)
		return (NULL);
	name[0] = BASE[0];
	name[1] = '\0';
	while (fichier_existe(name))
		increment_base(name, BASE_LENGTH);
	return (name);
}

/* int main(int argc, char **argv, char **env)
{
	 char *name;
	name = generate_name();
	if (name)
	{
		printf("Nom de fichier généré : %s\n", name);
		free(name);
	}
	else
		fprintf(stderr, "Erreur lors de la génération du nom de fichier.\n");
	return (0);
} */