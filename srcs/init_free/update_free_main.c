/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_free_main.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:20:49 by Matprod           #+#    #+#             */
/*   Updated: 2024/09/16 11:22:24 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_sig_int;

void	update_var_sigint(t_all **p)
{
	g_sig_int = 0;
	(*p)->exit_status = 127;
}

void	reset_signal_free(t_all *p)
{
	create_signal();
	free_here_docs(p->here_doc);
	free_ast(p->ast);
}
