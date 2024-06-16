/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:41:06 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/16 15:06:54 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void print_env(t_env *env)
{
    t_env *current;

    current = env;
    while (current != NULL)
    {
        printf("%s=%s\n", current->key, current->value);
        current = current->next;
    }
}

void	print_error_token(char *value)
{
	write(2, "bash: parse error near '", 24);
	write(2, value, ft_strlen(value));
	write(2, "'\n", 2);
}

void	print_error_token_special(char *value)
{
	if (!(ft_strcmp(value, ">")) || !(ft_strcmp(value, "<")) || !(ft_strcmp(value, "<<")) || !(ft_strcmp(value, ">>")))
	{
		write(2, "bash: parse error near ", 24);
		write(2, "'\\n'\n", ft_strlen("'\\n'\n"));
	}
	else
	{
		write(2, "bash: parse error near '", 24);
		write(2, value, ft_strlen(value));
		write(2, "'\n", 2);
	}
}
