/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 22:57:28 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/01 17:14:50 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_all	*init_all(char **env)
{
	t_all *p;
	
	p = malloc(sizeof(t_all));
	if (!p)
		return (NULL);
	p->env = env_to_struct(env);
	if (!p->env)
		return (free(p), NULL);
	p->sig = init_signal(0);
	if (!p->sig)
		return (free(p), NULL);
	p->ast = NULL;
	p->line = NULL;
	return (p);
}

t_sig	*init_signal(int nb)
{
    t_sig *sig;

	if (create_signal() == -1)
		return (NULL);
	sig = malloc(sizeof(t_sig));
	if (!sig)
		return (NULL);
	sig->sig_quit = nb;
	sig->sig_int = nb;
	sig->p_status = nb;
	return(sig);
}
