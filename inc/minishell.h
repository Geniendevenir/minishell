/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 11:15:24 by Matprod           #+#    #+#             */
/*   Updated: 2024/08/06 21:00:29 by allan            ###   ########.fr       */
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

# define BASE "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
# define BASE_LENGTH 62
# define MAX_FILENAME_LENGTH 165

//DONT CHANGE THE NUMBER
# define ERROR_MALLOC 1
# define ERROR_AND 2
# define ERROR_ENV 3
# define ERROR_DQUOTES 4
# define ERROR_SQUOTES 7
# define ERROR_SEMICOLON 5
# define ERROR_DUOSEMICOLON 8
# define ERROR_FILE 6

# define ERR_INFILE "Error: Opening Infile Failed\n"
# define ERR_OUTFILE "Error: Opening Outfile Failed\n"
# define ERR_PIPE "Error: Pipe Initialisation Failed\n"
# define ERR_FORK "Error: Fork Initialisation Failed\n"
# define ERR_CMD "Error: Command not found\n"
# define ERR_EXEC "Error: Command Execve Failed\n"

/*					 LEXER					*/

typedef struct s_index
{
	size_t	*i;
	size_t	j;
	bool	special;
	bool	error;
}				t_index;

typedef struct s_syntax
{
	int		openpar;
	bool	operator;
}				t_syntax;

typedef struct s_path
{
	char	*path_value;
	char	**env_paths;
	char	*final_path;
	char	*part_path;
	int		i;
}				t_path;

enum s_state{
	STATE_START,
	STATE_WHITESPACE,
	STATE_WORD,
	STATE_OPERATOR,
	STATE_EXIT_STATUS
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
	WORD_WTF,
	WORD_SQLIMITER, //dans le cas ou j'ai oublie un cas
};

typedef struct s_token
{
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
	int subshell;
	int exit_state;
	enum s_type type;
	enum s_state state;
	char		*value;
	struct s_ast *left;
	struct s_ast *right;
	struct s_ast *parent;
}				t_ast;

typedef struct s_ast_ptr
{
	t_ast	*root;
	t_ast	*current;
	t_ast	*last_ope;
	t_ast	*last_pipe;
	t_ast	*last_cmd;
}	t_ast_ptr;

typedef struct s_file
{
	int				fd;
	enum s_type		type;
	char			*data;
	struct s_file	*next;
}	t_file;

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
	t_ast	*ast;
	char	**here_doc;
	char	*line;
	int		line_num;
	t_sig	*sig;
	int		exit_status;
	int		max_pipe;
	int		curr_pipe;
	int		error;
	int		std_in;
	int		std_out;
	int		option;
}	t_all;

typedef struct s_word
{
	int redi_in;
	int redi_out;
	int append;
	int operator;
	int cmd;
} t_word;

typedef struct s_exec
{
	int				pipe;
	bool			redirectin;
	bool			redirectout;
	struct s_ast 	*in;
	struct s_ast 	*out;
	int				filein;
	int				fileout;
	char 			**command;
	char			*path;
}				t_exec;


//						EXECUTION                      //
int			executer(t_all *p, t_ast *current, char **env);

//ast_explorer
t_ast		*left_expand(t_all *p, t_ast *current);
t_ast		*get_next_operator(t_ast *current, t_ast	**prev);

//exec_check_cmd
int			get_command(t_ast *current, t_exec *exec);
int			command_size(t_ast *current);
char		**parse_command(t_ast *current, int size);
int			check_cmd(t_exec *exec, t_env *env);

//exec_cmd
int	exec_builtin(char **cmd, t_env **env_list);
int			exec_cmd(t_exec *exec, int *exit_status, char **env);
bool		is_builtin(char *cmd);

//exec_file
int			open_filein(t_exec *exec);
int			open_fileout(t_exec *exec);
int			open_files(t_exec *exec);
int			close_files(t_exec *exec, int std_in, int std_out);

//exec_get_path
char		*find_path(t_env *env);
char		*path_free(t_path *p, int *error, int option);
void		p_init(t_path *p);
char		*get_path(const char *cmd, t_env *env, int *error);

//exec_redirect
int			assign_redirect(t_ast *current, t_exec *exec);
void		redirect_pipe(t_ast *current, t_exec *exec);

//exec_utils
void		exec_init(t_exec *exec);
void		exec_free(t_exec *exec);
void		set_pipe(t_exec *exec, int set_pipe);
int			array_size(char **arr);
void		set_pipe(t_exec *exec, int set_pipe);


//////////////////////////////////////////////////////////

//				PARSER

int			parser(char *cmd_line, t_env *env, t_ast **ast, t_all **p);

//check_lexer
int			check_quotes(char *cmd_line);
int			check_semicolon(char *cmd_line);
int			skip_quotes(const char *cmd_line, int i, int option);

//lexer
bool 		lexer(char *cmd_line, t_token **token_list, int error);
int			tokenizer_one(const char *cmd_line, size_t *i, t_token **token_list);
int			tokenizer_two(const char *cmd_line, size_t *i, t_token **token_list);
int			tokenizer_three(const char *cmd_line, size_t *i, t_token **token_list);
int			tokenizer_four(const char *cmd_line, size_t *i, t_token **token_list);

//token_management
void 		token_init(t_token **token_list);
t_token 	*token_last(t_token *token_list);
bool		token_addback(t_token **token_list, char *value, int option);
void		token_addback_two(t_token **token_list, t_token *token);
bool		token_addnext(t_token **current, char *value);
int			token_free(t_token **token_list);

//tokenizer
bool		whitespace_token(const char *cmd_line, size_t *i, t_token **token_list);
bool		squote_token(const char *cmd_line, size_t *i, t_token **token_list);
bool		inpar_token(size_t *i, t_token **token_list);
bool		outpar_token(size_t *i, t_token **token_list);
bool		or_token(size_t *i, t_token **token_list);
bool		pipe_token(size_t *i, t_token **token_list);
int			and_token(const char *cmd_line, size_t *i, t_token **token_list);
bool		heredoc_token(size_t *i, t_token **token_list);
bool		inputre_token(size_t *i, t_token **token_list);
bool		outputapp_token(size_t *i, t_token **token_list);
bool		outputre_token(size_t *i, t_token **token_list);
bool		lexical_token(const char *cmd_line, size_t *i, t_token **token_list);
bool		word_token(const char *cmd_line, size_t *i, t_token **token_list); //split
int			env_token(const char *cmd_line, size_t *i, t_token **token_list);
void		env_special_token(t_token **token_list, int option);
bool		wildcard_token(const char *cmd_line, size_t *i, t_token **token_list);
bool		dquotes_token(const char *cmd_line, size_t *i, t_token **token_list);
bool		dquote_add_token(char *token_value, t_token **token_list, int option);
bool		dquotes_last_token(const char *cmd_line, t_index *index, t_token **token_list);
bool		env_dquotes(const char *cmd_line, t_index *index, t_token **token_list);
size_t		index_foward(size_t *j);

bool		limiter_token(const char *cmd_line, size_t *i, t_token **token_list);
bool		stop_limiter(const char *cmd_line, size_t *i, bool option);
bool		limiter_squote(const char *cmd_line, t_index *index, char **token_value);
bool		limiter_dquote(const char *cmd_line, t_index *index, char **token_value);
bool		limiter_word(const char *cmd_line, t_index *index, char **token_value);
bool		last_heredoc(t_token **token_list);
bool		create_limiter(const char *cmd_line, t_index *index, char **token_value);
bool		limiter_join(const char *cmd_line, t_index *index, char **token_value);

//utils
bool		is_whitespace(char c);
bool		is_word(char c);
int			is_env(char c, int option);
bool		is_valid_env(char c);
bool		is_freeable(char *value, int option);
bool		is_wildcard(const char *cmd_line, int i);
void		index_init(t_index *index, size_t *i, int option);

//print
void		token_print(t_token **token_list);
void		token_print_amazing(t_token **token_list);
void		amazing_printing(t_token *current, int i);
const char*	getToken_State(t_token *current);
const char	*getToken_Class(t_token *current);

//error_management
void		error_lexer(int error);
bool		error_syntax(t_token *current, int error);
void		error_expander(t_ast *current, int error);
void		error_executer(char *error, int option);
void		error_builtins(char *error, int option);

//here_doc
void		cleanbuffer(char *buffer);
int			prev_valo(char *buffer);
int			hdoc_process(int fd, t_token *limiter, t_all **p);
int			fill_here_doc(t_token **current, int max, t_all **p, int *nb);
void		here_doc(t_token **token_list, t_all **p);
void		warning(char *str, int nb);
int			quit_here_doc(int opt, t_all *p, int nb);
void		free_here_docs(char **here_docs);
int			here_doc_count(t_token *tok);
void		init_here_docs(t_token *token_list, t_all **p);
int			create_signal_here(t_all **p);
int			signals_hdoc(int opt, t_all **p);
int			fichier_existe(const char *name);
void		if_in_increment_base(size_t len, size_t *j, char *name);
void		increment_base(char *name, size_t len_base);
char		*generate_name(void);
char		*ft_strjoin_spe(char *s1, char const *s2);
void		bloquer_signal_eof(void);
void		restaurer_signal_eof(void);


/*								EXPANDER						*/
//split_word
t_ast		*replace_word(t_ast **root, t_ast *node, t_ast *new_node);
void		delete_word(t_ast **root, t_ast **node);
bool		modify_word(t_ast **node, t_token *token_list);
int			split_word(t_all *p, t_ast **current);
int			split_one(const char *cmd_line, size_t *i, t_token **token_list);
int			split_two(const char *cmd_line, size_t *i, t_token **token_list);
bool		limit_word(char c);
int			word_management(t_ast **root, t_ast **current, t_token	*token_list);

//handle_wildcard
int			handle_wildcard(t_ast **current, t_token **token_list);
bool		insert_word(t_ast **node, t_token *token);

bool		expander(t_token **token_list, t_all *p, int error);

//expand env
int			expand_env(t_token **token_list, t_env **env, int exit_status);
bool		find_first_env(t_token **current, t_env **env, int exit_status);
bool		find_next_env(t_token **current, t_env **env, int exit_status);
void		remove_token(t_token **current, bool option);
bool		replace_token(t_token *token,  char *new_value);
void		remove_all_env(t_token **token_list, int exit_status);
bool		replace_exit_status(t_token *token, int exit_status);
bool		expand_all_exit(t_token **token_list, int exit_status);

//relink
int			relink_token(t_token **token_list, t_token *current, int error);
t_token		*relink_word(t_token *current, t_token **new_list, int *error);
bool		relink_operator(t_token *current, t_token **new_list);
bool		add_word(t_token **new_list, char *word, int option);
void		relink_word_init(char **word, char **new_word, int *wildcard);

//wildcard
int			expand_wildcard(t_token **token_list, int *error);
int			find_wildcard(char *wildcard, t_token *current, int *error);
void		match_init(char *wildcard, char *file_name, t_wildcard *match);
bool		file_match(t_wildcard match);
bool		add_file(t_token **current, char *file_name, bool found);
bool		wildcard_return(DIR **d);
	
void		print_envv(t_env **env);

//////////////////////////////////////////////////////////


/*						AST					*/

t_ast		*parse_expression(t_token **token_list, int sub_shell);
t_ast		*parse_subexpression(t_token **tokens, int sub_shell);
t_ast		*open_parenthesis(t_token **tokens, t_ast	*current, int sub_shell);
t_ast		*close_parenthesis(t_token **tokens, t_ast* root);
t_ast		*handle_builtin_and_cmd(t_token **tokens, t_ast_ptr	**list, int sub_shell);
t_ast		*handle_option(t_token **tokens, t_ast_ptr **list, int sub_shell);
t_ast		*create_node(t_token *token, int subshell);
void		handle_parenthesis_open(t_token **tokens, t_ast_ptr **list, int sub_shell);
void		ope_pipe_redirect(t_token **tokens, t_ast_ptr **list, int sub_shell);
void		handle_and_or_root_priority(t_token **tokens, t_ast_ptr **list, int sub_shell);
void		handle_pipe(t_token **tokens, t_ast_ptr **list, int sub_shell);
void		while_in_handle_redirect(t_ast_ptr **list, t_ast **new_node);
void		handle_redirect(t_token **tokens, t_ast_ptr **list, int sub_shell);
void		handle_builtin_cmd_or_option(t_token **tokens, t_ast_ptr **list, int sub_shell);
void		swap_child_left(t_ast	*current, t_ast	*new_node);
void		swap_child_right(t_ast	*current, t_ast	*new_node);
void		swap_child_left_with_else(t_ast	*current, t_ast	*new_node);
void		swap_child_right_with_else(t_ast	*current, t_ast	*new_node);
void		part_handle_option(t_ast_ptr **list, t_ast **new_node, t_ast **temp);
void		while_in_handle_pipe(t_ast **current, t_ast **new_node, t_ast *save_operator);
void		if_last_ope_exist(t_ast **new_node, t_ast_ptr **list);
void		if_no_save_operator(t_ast **current, t_ast **new_node,
t_ast		**save_operator, t_ast **save_pipe);
void		init_pointer_ast(t_ast_ptr **list);
void		free_token_and_next(t_token **tokens, t_token **temp);
void		get_first_parent(t_ast_ptr **list);
bool		is_pipe(t_token **tok);
bool		is_ope(t_token **tokens);
bool		if_cmd_or_option(t_token **tokens);
bool		is_redirect(t_token **tok);
bool		is_redirect_enum(enum s_type word);
void		free_list_ptr(t_ast_ptr **list, t_ast **temp_free, int option);



/*					SIGNALS					*/

int			event(void);
int			create_signal(void);
void		sighandler(int signal);
int			stop_signals(void);

/*					SYNTAX AND WORD					*/
int			check_word_part_cmd(char *word, t_word *boolean);
int			check_word_part_append(t_word *boolean);
int			check_word_part_rediout(t_word *boolean);
int			check_cmd_exist(char *word, t_env *env);
int			check_builtin(char *word);
int			check_file(char *word);
int			check_absolute_path_cmd(char *word);
int			check_word_part(char *word, t_word *boolean);
enum s_type	check_word(char *word, t_word *boolean);
bool		define_word(t_token **token_list, t_word *boolean);
bool		is_operator(enum s_type type, int option);
int			double_operator(t_token *c);
int			check_first_token(t_token *c);
bool		check_syntax(t_token *current);

bool		check_parenthesis(t_token *current, t_syntax syntax, int *skip);
bool		closepar_error(t_token *current, t_syntax syntax, int *skip);
bool		openpar_error(t_token *current, t_syntax syntax, int *skip);
bool		is_parenthesis_error(t_token *current, t_syntax syntax, int option);
bool		check_current_parenthesis(t_token *current, int option);

/*					 ENV					*/


/*					 INIT					*/
t_all		*init_all(char **env);
void		init_signal(int nb, t_all **p);
void		init_t_word(t_word *word);

/*					 FREE					*/

void		free_env(t_env *envp);
void		free_all(t_all *p);
void		free_array(char **array);
void		free_ast(t_ast *node);

/*					 UTILS					*/


char		*ft_strndup(char *str, int n);
int			ft_strcmp(char *s1, char *s2);
void 		print_env(t_env *env);
void		print_error_token(t_token *current);
void		print_error_token_special(char *value);
void		print_error_cmd_not_found(t_token *current);
void		printAST(t_ast* node, int level);
const char* getAST_Class(t_ast *current);
void		print_tab(char **command);

/*					BUILTINS				*/
//ENV
t_env		*env_to_struct(char **env);
void		ft_env_add_back(t_env **lst, t_env *new);
//PWD
int			ft_pwd(char *option);
//CD
int			ft_cd(char *path);
char		*relative_path(DIR	*d, char *path, int *error);
char		*cd_match(char *cur_dir, char *try_dir, int *error);
//EXPORT
int			ft_export(char **new_env, t_env **env_list);
int 		split_env(char *new_env, int len, t_env **env_list);
t_env		*env_init(void);
int			export_free(t_env **add_env, int option);
int			valid_export(char *new_env);
//EXIT
int			ft_exit(char **commande, bool child);
int			check_is_num(char *exit_status);
int			check_size(long long int *exit_status);
void 		check_is_child(char *commande, bool child, int error);
//ECHO
bool		check_echo(char *str);
//static int	conditions_echo(char *cmd);
int			ft_echo(char **cmd);

int			main(int argc, char **argv, char **env);
char		*minishell(t_all *p, char **env);

//extern int	sig_int;
/*						AST	TRY				*/

#define PRECEDENCE_LOWEST 1
#define PRECEDENCE_AND_OR 2
#define PRECEDENCE_PIPE 3
#define PRECEDENCE_REDIRECTION 4
#define PRECEDENCE_HIGHEST 5

#endif