/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 15:42:25 by Matprod           #+#    #+#             */
/*   Updated: 2024/09/13 19:11:40 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_hdoc(int fd, char *buffer)
{
	write(fd, buffer, ft_strlen(buffer));
	write(fd, "\n", 1);
	free(buffer);
}

bool here_doc_check_file(t_all *p, t_token *token_list)
{
	int i;

	i = 0;
	while(p && p->here_doc && p->here_doc[i])
	{
		if (access(p->here_doc[i], F_OK) == 0)
			i++;
		else
		{
			token_free(&token_list);
			free_here_docs(p->here_doc);
			return (ft_putstr_fd("Error delete file here_doc\n", 2), 1);
		}
	}
	return(0);
}