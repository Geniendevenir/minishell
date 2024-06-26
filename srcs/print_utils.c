/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:41:06 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/26 16:04:29 by Matprod          ###   ########.fr       */
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

void	print_error_token(t_token *current)
{
	if (current->type == 12 && current->next->type == 13)
		write(2, "bash: syntax error near unexpected token `)'\n", 45);
	else
	{
		write(2, "bash: syntax error near unexpected token `", 42);
		write(2, current->value, ft_strlen(current->value));
		write(2, "'\n", 2);
	}
}

void	print_error_token_special(char *value)
{
	if (!(ft_strcmp(value, ">")) || !(ft_strcmp(value, "<")) 
		|| !(ft_strcmp(value, "<<")) || !(ft_strcmp(value, ">>")))
	{
		write(2, "bash: syntax error near unexpected token `newline'\n", 51);
	}
	else
	{
		write(2, "bash: syntax error near unexpected token `", 42);
		write(2, value, ft_strlen(value));
		write(2, "'\n", 2);
	}
}
void print_error_cmd_not_found(t_token *current)
{
	write(2, current->value, ft_strlen(current->value));
	write(2, ": command not found\n", 20);
}
