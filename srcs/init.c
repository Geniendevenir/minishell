/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 22:57:28 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/10 23:08:59 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_all	*init_all(char **env)
{
	t_all	*p;

	p = malloc(sizeof(t_all));
	if (!p)
		return (NULL);
	p->env = env_to_struct(env);
	if (!p->env)
		return (free(p), NULL);
	p->sig = malloc(sizeof(t_sig));
	if (!p->sig)
		return (free_env(p->env), free(p), NULL);
	if (create_signal() == -1)
		return (free_env(p->env), free(p->sig), free(p), NULL);
	p->sig->sig_quit = 0;
	p->sig->sig_int = 0;
	p->sig->p_status = 0;
	p->sig->cmd_stat = 0;
	p->line_num = 1;
	p->here_doc = NULL;
	p->ast = NULL;
	p->line = NULL;
	p->exit_status = 0;
	p->max_pipe = 0;
	p->curr_pipe = 0;
	p->error = 0;
	return (p);
}

void	init_signal(int nb, t_all **p)
{
	if ((*p)->sig)
		free((*p)->sig);
	(*p)->sig = malloc(sizeof(t_sig));
	if (!(*p)->sig)
		return ;
	if (create_signal() == -1)
		return ;
	(*p)->sig->sig_quit = nb;
	(*p)->sig->sig_int = nb;
	(*p)->sig->p_status = nb;
	(*p)->sig->cmd_stat = nb;
}
