/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 13:58:59 by allan             #+#    #+#             */
/*   Updated: 2024/07/03 18:09:34 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*		ERROR MANAGEMENT		

Minishell -> Nouveau Prompt
	↓
  PARSER
  1 - Empty Cmd_line			->							-> Return (nothing);	-> Back to Minishell
  2 - Token_List Malloc Failed	->							-> Return (nothing);	-> Back to Minishell
  3 - Lexer						-> No Free*					-> Return (nothing);	-> Back to Minishell
  4 - Expander					-> No Free*					-> Return (nothing);	-> Back to Minishell
  5 - Define_word				->
  6 - parseExpression			->
  * = Already Freed in a subfuncion
	↓
  LEXER
  1 - Invalid Quotes			-> Token_free + Error_Lexer	-> Return (1);			-> Back to Parser
  2 - Invalid semicolon			-> Token_free + Error_Lexer	-> Return (1);			-> Back to Parser
  3 - Tokenizer					-> Token_free + Error_Lexer -> Return (1);			-> Back to Parser
		↓
	TOKENIZER
	1 - GENERAL Token_addback Malloc Failed					-> Return (1);			-> Back to Lexer
	2 - GENERAL Token_last Empty								-> Return (1);			-> Back to Lexer  (NECESSAIRE ?)
	3 - ENV: Invalid ENV										-> Return (3);			-> Back to Lexer
	4 - WORD: substr Malloc Failed							-> Return (1);			-> Back to Lexer
	5 - WORD: Token_Last Empty	-> Free token_value			-> Return (1);			-> Back to Lexer
	6 - Token_addback Failed		-> No Free*					-> Return (1);			-> Back to Lexer
		(token_addback)
		1 - Token_list vide			-> Free token_value			-> Return (1);			-> Back to Tokenizer (NECESSAIRE ?)
		2 - Token Malloc Failed		-> Free token_value			-> Return (1);			-> Back to Tokenizer
		3 - Strdup Malloc Failed		-> Free token_value			-> Return (1);			-> Back to Tokenizer
		↓
		D_QUOTES
		1 - endquotes					-> No Free*					-> Return (1);			-> Back to Lexer
		2 - dquotes_last_token		-> No Free*					-> Return (1);			-> Back to Lexer
			(env_dquotes)
			1 - first substr failed									-> Return (1);			-> Back to Dquotes
			2 - first add_token			-> No Free*					-> Return (1);			-> Back to Dquotes
			3 - second substr failed								-> Return (1);			-> Back to Dquotes
			4 - second add_token		-> No Free*					-> Return (1);			-> Back to Dquotes
				(dquote_add_token)
				1 - Token_addback Failed	-> No Free*					-> Return (1);			-> Back to env_dquotes
				2 - token_last empty list	-> Free token_value			-> Return (1);			-> Back to env_dquotes
			(dquotes_last_token)
			1 - substr failed										-> Return (1);			-> Back to Dquotes
			2 - dquotes_add_token		-> No Free*					-> Return (1);			-> Back to Dquotes
	↓
  EXPANDER
  1 - expand_env				-> Free token_list	+ Error_Lexer	-> Return (1);				-> Back to Lexer
  2 - relink_token				-> 
  3 -
	EXPAND_ENV
	1 - find_first_env										-> Return (1);				-> Back to Expander
	2 - find_next_env										-> Return (1);				-> Back to Expander
		(find_first_env)
		1 - replace token strdup								-> Return (1);			-> Back to expand_env
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
	else if (error == 7)
		write(2, "bash: syntax error near unexpected token `\''\n", 46);
	else if (error == 5)
		write(2, "bash: syntax error near unexpected token ';'\n", 45);
	else if (error == 6)
		write(2, "Error: Could not open directory '.'\n", 37); //check true error (find wildcard function)
}

bool	error_syntax(t_token *current, int error)
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
		write(2, "bash: no '&&' or '||' token found in between parenthesis\n", 57);
	return (1);
	//if (current->type == 12 && current->next->type == 13)
	/* if (error == 2)
		write(2, "bash: syntax error near unexpected token `)'\n", 45); */
}