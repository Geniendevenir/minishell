# Standard
NAME				= minishell

# Directories
LIBFT				= ./libft/libft.a
INC					= inc/
SRC_DIR				= srcs/
OBJ_DIR				= obj/

# Compiler and CFlags
CC					= cc
CFLAGS				= -Wall -Wextra -I
RM					= rm -f
READLINE			= -lreadline

# Source Filesmake
MINI_SHELL_DIR		=	$(SRC_DIR)

# Concatenate all source files
SRCS 				=   srcs/env.c\
						srcs/free.c\
						srcs/main.c \
						srcs/init.c\
						srcs/print_utils.c \
						srcs/signals.c\
						srcs/utils.c\
						srcs/parsing/syntax/word.c\
						srcs/parsing/syntax/check_word.c\
						srcs/parsing/syntax/check_word_utils.c\
						srcs/parsing/syntax/check_syntax.c\
						srcs/parsing/syntax/check_parenthesis.c\
						srcs/parsing/parser.c\
						srcs/parsing/lexer/check_lexer.c\
						srcs/parsing/lexer/lexer.c\
						srcs/parsing/lexer/token_management.c\
						srcs/parsing/lexer/tokenizer_one.c\
						srcs/parsing/lexer/tokenizer_two.c\
						srcs/parsing/lexer/tokenizer_three.c\
						srcs/parsing/lexer/tokenizer_four.c\
						srcs/parsing/lexer/tokenizer_utils.c\
						srcs/parsing/error_parser.c\
						srcs/parsing/expander/expander.c\
						srcs/parsing/expander/remove_env.c\
						srcs/parsing/expander/expander_utils.c\
						srcs/parsing/expander/expand_env.c\
						srcs/parsing/expander/relink_token.c\
						srcs/parsing/expander/expand_wildcard.c\
						srcs/parsing/ast/ast.c\
						srcs/parsing/ast/ast_handle_builtin_and_cmd.c\
						srcs/parsing/ast/ast_swap_child.c\
						srcs/parsing/ast/ast_handle_and_or.c\
						srcs/parsing/ast/ast_handle_pipe.c\
						srcs/parsing/ast/ast_handle_redirect.c\
						srcs/parsing/ast/ast_handle_parenthesis.c\
						srcs/parsing/ast/ast_test.c\
						srcs/parsing/ast/print_ast.c\
						srcs/builtins/ft_cd.c\
						srcs/builtins/ft_export.c\
						srcs/builtins/ft_exit.c\
						srcs/execution/if_utils_exec.c\
						srcs/execution/parcour_ast.c\
						srcs/execution/exec_operator.c\
						srcs/execution/exec_cmd_and_builtins.c\
						srcs/execution/exec_builtins.c\
						srcs/execution/executer.c\
						srcs/hate_the_norm.c\

# Apply the pattern substitution to each source file in SRC and produce a corresponding list of object files in the OBJ_DIR
OBJ 				= $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SRCS))

# Build rules
start:				
					@make all

$(LIBFT):
					@make -C ./libft

all: 				$(NAME)

$(NAME): 			$(OBJ) $(LIBFT)
					@$(CC) $(CFLAGS) $(INC) $(OBJ) $(READLINE) $(LIBFT) -o $(NAME)

# Compile object files from source files
$(OBJ_DIR)%.o:		$(SRC_DIR)%.c 
					@mkdir -p $(@D)
					@$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
					@$(RM) -r $(OBJ_DIR)
					@make clean -C ./libft

fclean: clean
	@$(RM) $(NAME)
	@$(RM) $(LIBFT)

re: fclean all

# Phony targets represent actions not files
.PHONY: 			start all clean fclean re
