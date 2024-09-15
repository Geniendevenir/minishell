/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 13:58:59 by allan             #+#    #+#             */
/*   Updated: 2024/09/15 17:00:58 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_lexer(int error)
{
	if (error == 1)
		write(2, "Error: Malloc Failed\n", 21);
	else if (error == 2)
		write(2, "bash: syntax error near unexpected token `&'\n", 45);
	else if (error == 3)
		write(2, "bash: syntax error near unexpected token `$'\n", 45);
	else if (error == 4)
		write(2, "bash: syntax error near unexpected token `\"'\n", 46);
	else if (error == 7)
		write(2, "bash: syntax error near unexpected token `\''\n", 46);
	else if (error == 5)
		write(2, "bash: syntax error near unexpected token ';'\n", 45);
	else if (error == 8)
		write(2, "bash: syntax error near unexpected token ';;'\n", 46);
	else if (error == 6)
		write(2, "Error: Could not open directory '.'\n", 37);
}

void	error_syntax(t_token *current, int error)
{
	if (error == 1)
	{
		write(2, "bash: syntax error near unexpected token '", 42);
		write(2, current->value, ft_strlen(current->value));
		write(2, "'\n", 2);
	}
	if (error == 2)
		write(2, "bash: syntax error near unexpected token '('\n", 45);
	if (error == 3)
		write(2, "bash: syntax error near unexpected token ')'\n", 45);
	if (error == 4)
		write(2, "bash: no '&&' or '||' token found in between parenthesis\n",
			57);
	if (error == 5)
		write(2, "bash: syntax error near unexpected token 'newline'\n", 51);
	if (error == 6)
		write(2, "bash: '|' token found in between parenthesis\n", 46);
	if (error == 7)
		write(2, "bash: '|' token found just after parenthesis\n", 45);
}

void	error_expander(t_ast *current, int error)
{
	if (error == 1)
	{
		write(2, "bash: ", 6);
		write(2, current->value, ft_strlen(current->value));
		write(2, ": ambiguous redirect\n", 21);
	}
}

void	error_executer(char *error, int option)
{
	if (option == 1)
	{
		write(2, "bash: ", 6);
		write(2, error, ft_strlen(error));
		write(2, ": No such file or directory\n", 28);
	}
	else if (option == 2)
		write(2, "Error: Output File Opening Failed\n", 34);
	else if (option == 3)
		write(2, "Error: Input File Opening Failed\n", 33);
	else if (option == 4)
		write(2, "Error: Malloc Allocation Failed\n", 32);
	else if (option == 5)
		write(2, "Error: Fork Failed\n", 19);
	else if (option == 6)
		write(2, "Error: Dup2 Initialisation Failed\n", 35);
	else if (option == 7)
		write(2, "Error: Opendir Error\n", 21);
	else if (option == 8)
		write(2, "Error: Fork Initialisation Failed\n", 34);
	else if (option == 9)
		write(2, "Error: Waitpid Initialisation Failed\n", 37);
	else if (option == 10)
		write(2, "Error: Pipe Initialisation Failed\n", 34);
}

void	error_builtins(char *error, int option)
{
	if (option == 1)
		write(2, "Error: lstat function Failed", 6);
	else if (option == 2)
	{
		write(2, "bash: ", 6);
		write(2, error, ft_strlen(error));
		write(2, ": Not a directory\n", 18);
	}
	else if (option == 3)
		write(2, "cd : too many arguments\n", 24);
}
