/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 11:15:24 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/15 13:31:17 by allan            ###   ########.fr       */
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
# include <../libft/inc/libft.h>
# include <../libft/inc/ft_printf.h>
# include <../libft/inc/get_next_line.h>


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


extern t_sig	g_sig;

/*					 LEXER					*/

typedef struct s_index {
	int	*i;
	int	*j;
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
	TOKEN_COUNT
};

typedef struct s_token {
	enum s_type type;
	enum s_state state;
	char *value;
	long len;
	struct s_token *next;
}				t_token;

//check_lexer
bool	check_quotes(char *cmd_line);
bool	check_semicolon(char *cmd_line);

//lexer
void	parser(char *cmd_line, t_env *env);
bool	lexer(char *cmd_line, t_token **token_list);
int		tokenizer_partwo(const char *cmd_line, size_t *i, t_token **token_list);
int		tokenizer(const char *cmd_line, size_t *i, t_token **token_list);

//token_management
void	token_print(t_token **token_list);
void	token_print_amazing(t_token **token_list);
bool 	token_init(t_token *token_list);
t_token *token_last(t_token *token_list);
bool	token_addback(t_token **token_list, char *value, bool option);
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

//utils
bool	is_whitespace(char c);
bool	is_word(char c);
bool	is_env(char c);
bool	is_valid_env(char c);
bool	is_freeable(char *value, bool option);
bool	is_wildcard(const char *cmd_line, int i);

//error
void	error_lexer(int error);

void		amazing_printing(t_token *current, int i);
const char	*getToken_Class(t_token *current);


/*								EXPANDER						*/
bool	expander(t_token **token_list, t_env *env);
int		expand_env(t_token **token_list, t_env **env);
bool	find_first_env(t_token **current, t_env **env);
bool	find_next_env(t_token **current, t_env **env);
void	remove_token(t_token **current, bool option);
bool	replace_token(t_token *token,  char *new_value);
void	remove_all_env(t_token **token_list);

bool	expand_quoted_env(t_token **token_list, t_env *env); // to delete

void	print_envv(t_env **env);

///////////////

/*					SIGNALS					*/

int	event(void);
int	create_signal(void);
/*					CHECK					*/

int	check_cmd_exist(char *word, t_env *env);

/*					 ENV					*/

t_env	*env_to_struct(char **env);

/*					 INIT					*/
t_all	*init_all(char **env);
t_sig	*init_signal(int nb);

/*					 FREE					*/

void	free_env(t_env *envp);
void	free_all(t_all *p);
void	free_array(char **array);

/*					 UTILS					*/


char	*ft_strndup(char *str, int n);
int		ft_strcmp(const char *s1, const char *s2);
void	print_env(t_env *env);

#endif