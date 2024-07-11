/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 10:58:09 by allan             #+#    #+#             */
/*   Updated: 2024/07/11 18:46:06 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_envv(t_env **env)
{
	t_env	*current;

	current = *env;
	while (current)
	{
		if (current->key)
			printf("%s\n", current->key);
		current = current->next;
	}
}
