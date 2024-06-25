/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 11:15:24 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/25 17:27:05 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdbool.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <limits.h>
# include <string.h>
# include <signal.h>
# include <curses.h>
# include <term.h>
# include <termios.h>
# include <dirent.h>
# include <sys/ioctl.h>
# include <limits.h>
# include <errno.h>
# include <poll.h>
# include <sys/msg.h>
# include "../libft/inc/libft.h"
# include "../libft/inc/ft_printf.h"
# include "../libft/inc/get_next_line.h"

typedef struct s_env
{
	char			*key;
	char			*value;
	int				code;
	struct s_env	*next;
}	t_env;

typedef struct s_sig
{
	int		sig_quit;
	int		sig_int;
	int		p_status;
	int		cmd_stat;
}	t_sig;

typedef struct s_all
{
	t_env	*env;
	char	*line;
	t_sig	*sig;
}	t_all;

typedef struct s_word
{
	int redi_in;
	int redi_out;
	int here_doc;
	int append;
	int operator;
	int cmd;
} t_word;

extern t_sig	g_sig;

/*					 LEXER					*/

typedef struct s_index {
	size_t	*i;
	size_t	j;
}				t_index;

enum s_state{
	STATE_START,
	STATE_WHITESPACE,
	STATE_WORD,
	STATE_OPERATOR
};

enum s_type{
	NOT_DEFINE,
	TOKEN_WORD,
	TOKEN_DQUOTES,
	TOKEN_SQUOTES,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_PIPE,
	TOKEN_REDIRECTIN,
	TOKEN_REDIRECTOUT,
	TOKEN_HEREDOC,
	TOKEN_APPENDOUT,
	TOKEN_LIMITER,
	TOKEN_OPENPAR,
	TOKEN_CLOSEPAR,
	TOKEN_WHITESPACE,
	TOKEN_ENV,
	TOKEN_WILDCARD,
	TOKEN_COUNT,
	WORD_FILEIN,
	WORD_FILEOUT,
	WORD_FILEOUT_APPEND,
	WORD_BUILTIN,
	WORD_ABSPATH,
	WORD_CMD,
	WORD_OPTION, // option / argument d'une commande
	WORD_LIMITER,
	WORD_STRING,
	WORD_ERROR, //ERREUR (le WORD n'est pas classifie)
	WORD_WTF, //dans le cas ou j'ai oublie un cas
};

typedef struct s_token {
	enum s_type type;
	enum s_state state;
	char *value;
	long len;
	struct s_token *next;
}				t_token;

typedef struct s_wildcard {
	const char *file_name;
    const char *wildcard;
    const char *star;
    const char *backtrack;
}				t_wildcard;

typedef struct s_ast {
	enum s_type type;
	char		*value;
	struct s_ast *left;
	struct s_ast *right;
	struct s_ast *parent;
}				t_ast;
//check_lexer
bool	check_quotes(char *cmd_line);
bool	check_semicolon(char *cmd_line);

//lexer
void	parser(char *cmd_line, t_env *env);
bool	lexer(char *cmd_line, t_token **token_list);
int		tokenizer_one(const char *cmd_line, size_t *i, t_token **token_list);
int		tokenizer_two(const char *cmd_line, size_t *i, t_token **token_list);
int		tokenizer_three(const char *cmd_line, size_t *i, t_token **token_list);
int		tokenizer_four(const char *cmd_line, size_t *i, t_token **token_list);

//token_management
void	token_print(t_token **token_list);
void	token_print_amazing(t_token **token_list);
bool 	token_init(t_token *token_list);
t_token *token_last(t_token *token_list);
bool	token_addback(t_token **token_list, char *value, int option);
bool	token_addnext(t_token **current, char *value);
void	token_free(t_token **token_list);

//tokenizer
bool	whitespace_token(const char *cmd_line, size_t *i, t_token **token_list);
bool	squote_token(const char *cmd_line, size_t *i, t_token **token_list);
bool	inpar_token(size_t *i, t_token **token_list);
bool	outpar_token(size_t *i, t_token **token_list);
bool	or_token(size_t *i, t_token **token_list);
bool	pipe_token(size_t *i, t_token **token_list);
int		and_token(const char *cmd_line, size_t *i, t_token **token_list);
bool	heredoc_token(size_t *i, t_token **token_list);
bool	inputre_token(size_t *i, t_token **token_list);
bool	outputapp_token(size_t *i, t_token **token_list);
bool	outputre_token(size_t *i, t_token **token_list);
bool	lexical_token(const char *cmd_line, size_t *i, t_token **token_list);
int		env_token(const char *cmd_line, size_t *i, t_token **token_list);
bool	wildcard_token(const char *cmd_line, size_t *i, t_token **token_list);
bool	dquotes_token(const char *cmd_line, size_t *i, t_token **token_list);
bool	dquote_add_token(char *token_value, t_token **token_list, bool option);
bool	dquotes_last_token(const char *cmd_line, t_index *index, t_token **token_list);
bool	env_dquotes(const char *cmd_line, t_index *index, t_token **token_list);


//utils
bool	is_whitespace(char c);
bool	is_word(char c);
bool	is_env(char c);
bool	is_valid_env(char c);
bool	is_freeable(char *value, int option);
bool	is_wildcard(const char *cmd_line, int i);

//error
void	error_lexer(int error);
void		amazing_printing(t_token *current, int i);
const char	*getToken_Class(t_token *current);


/*								EXPANDER						*/
bool	expander(t_token **token_list, t_env *env);
//expand env
int		expand_env(t_token **token_list, t_env **env);
bool	find_first_env(t_token **current, t_env **env);
bool	find_next_env(t_token **current, t_env **env);
void	remove_token(t_token **current, bool option);
bool	replace_token(t_token *token,  char *new_value);
void	remove_all_env(t_token **token_list);

//relink
int		relink_token(t_token **token_list, t_token *current, int *error);
t_token	*relink_word(t_token *current, t_token **new_list, int *error);
bool	relink_operator(t_token *current, t_token **new_list);
bool	add_word(t_token **new_list, char *word, bool option);
void	relink_word_init(char **word, char **new_word, bool *wildcard);

//wildcard
int		expand_wildcard(t_token **token_list, int *error);
int		find_wildcard(char *wildcard, t_token *current, int *error);
void	match_init(char *wildcard, char *file_name, t_wildcard *match);
bool	file_match(t_wildcard match);
bool	add_file(t_token **current, char *file_name, bool found);

void	print_envv(t_env **env);

//////////////////////////////////////////////////////////

/*						AST					*/

t_ast* createNode(enum s_type type, char* value);
t_ast* handleOpenParenthesis(t_token **tokens, t_ast* current);
t_ast* handleCloseParenthesis(t_token **tokens, t_ast* root);
t_ast* handleRedirect(t_token **tokens, t_ast* root);
t_ast* handlePipe(t_token **tokens, t_ast* root);
t_ast* handleAndOr(t_token **tokens, t_ast* root);
t_ast* handleBuiltinCmdQuotes(t_token **tokens, t_ast* current);
t_ast* handlePipe(t_token **tokens, t_ast* root);
t_ast* handleRedirect(t_token **tokens, t_ast* root);
t_ast* handleOption(t_token **tokens, t_ast* current);
t_ast* parseExpression(t_token **tokens);
t_ast* parseSubexpression(t_token **tokens);
void printAST(t_ast* node, int level);

const char* getAST_Class(t_ast *current);

/*					SIGNALS					*/

int			event(void);
int			create_signal(void);
/*					CHECK					*/

int			check_word_part_cmd(char *word, t_word *boolean, t_env *env);
int			check_word_part_append(t_word *boolean);
int			check_word_part_rediout(t_word *boolean);
bool		check_double_syntax(t_token **token_list);
bool		check_syntax(t_token **token_list);
int			check_cmd_exist(char *word, t_env *env);
int			check_builtin(char *word);
int			check_file(char *word);
int			check_absolute_path_cmd(char *word);
int			check_word_part(char *word, t_word *boolean, t_env *env);
enum s_type	check_word(char *word, t_word *boolean, t_env *env);
bool		define_word(t_token **token_list, t_word *boolean, t_env *env);

/*					 ENV					*/

t_env	*env_to_struct(char **env);

/*					 INIT					*/
t_all	*init_all(char **env);
t_sig	*init_signal(int nb);
void	init_t_word(t_word *word);

/*					 FREE					*/

void	free_env(t_env *envp);
void	free_all(t_all *p);
void	free_array(char **array);

/*					 UTILS					*/


char	*ft_strndup(char *str, int n);
int		ft_strcmp(char *s1, char *s2);
void 	print_env(t_env *env);
void	print_error_token(t_token *current);
void	print_error_token_special(char *value);
void	print_error_cmd_not_found(t_token *current);



/*						AST	TRY				*/

#define PRECEDENCE_LOWEST 1
#define PRECEDENCE_AND_OR 2
#define PRECEDENCE_PIPE 3
#define PRECEDENCE_REDIRECTION 4
#define PRECEDENCE_HIGHEST 5

#endif