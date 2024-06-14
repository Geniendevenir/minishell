/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 10:58:09 by allan             #+#    #+#             */
/*   Updated: 2024/06/07 11:03:53 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_envv(t_env **env)
{
	t_env *current;

	current = *env;
	while (current)
	{
		if (current->key)
			printf("%s\n", current->key);
		current = current->next;
	}
}