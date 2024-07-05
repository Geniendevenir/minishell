/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 22:57:28 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/05 15:05:30 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_all	*init_all(char **env, int *exit_status)
{
	t_all *p;
	
	p = malloc(sizeof(t_all));
	if (!p)
		return (NULL);
	p->env = env_to_struct(env);
	if (!p->env)
		return (free(p), NULL);
	p->sig = init_signal(0, exit_status);
	if (!p->sig)
		return (free(p), NULL);
	p->ast = NULL;
	p->line = NULL;
	return (p);
}

t_sig	*init_signal(int nb, int *exit_status)
{
    t_sig *sig;

	if (create_signal(exit_status) == -1)
		return (NULL);
	sig = malloc(sizeof(t_sig));
	if (!sig)
		return (NULL);
	sig->sig_quit = nb;
	sig->sig_int = nb;
	sig->p_status = nb;
	return(sig);
}
