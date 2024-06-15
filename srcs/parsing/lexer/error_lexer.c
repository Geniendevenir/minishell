/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 13:58:59 by allan             #+#    #+#             */
/*   Updated: 2024/06/15 17:09:07 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*		ERROR MANAGEMENT		

Minishell
	↓
  parser
  1 - Empty Cmd_line	-> No Free		-> Return (nothing);		-> Back to Minishell
  2 - Error de Malloc	-> No Free		-> Return (nothing);		-> Back to Minishell
	↓
  lexer
  3 - Invalid Quotes/ Semicolon -> Token_free + Error Msg	-> Return (1);		-> Back Parser -> Back Minishell -> Nouveau Prompt
  
  
*/

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
	else if (error == 5)
		write(2, "bash: syntax error near unexpected token ';'\n", 45);
}