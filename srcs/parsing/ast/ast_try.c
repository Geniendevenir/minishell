/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_try.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 12:51:57 by allan             #+#    #+#             */
/*   Updated: 2024/06/25 21:10:14 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void printAST(t_ast* node, int level) {
    if (node == NULL) return;

    // Print the right subtree first (with increased level)
    if (node->right) {
        printAST(node->right, level + 1);
    }

    // Print current node with proper indentation
    for (int i = 0; i < level; i++) printf("    ");
    printf("%s: %s\n", getAST_Class(node), node->value);

    // Print the left subtree (with increased level)
    if (node->left) {
        printAST(node->left, level + 1);
    }
}

const char* getAST_Class(t_ast *current) {
    const char* Token_Class[] = {
        "NOT_DEFINE",
        "TOKEN_WORD",
        "TOKEN_DQUOTES",
        "TOKEN_SQUOTES",
        "TOKEN_AND",
        "TOKEN_OR",
        "TOKEN_PIPE",
        "TOKEN_REDIRECTIN",
        "TOKEN_REDIRECTOUT",
        "TOKEN_HEREDOC",
        "TOKEN_APPENDOUT",
        "TOKEN_LIMITER",
        "TOKEN_OPENPAR",
        "TOKEN_CLOSEPAR",
        "TOKEN_WHITESPACE",
        "TOKEN_ENV",
        "TOKEN_WILDCARD",
        "TOKEN_COUNT",
        "WORD_FILEIN",
        "WORD_FILEOUT",
        "WORD_FILEOUT_APPEND",
        "WORD_BUILTIN",
        "WORD_ABSPATH",
        "WORD_CMD",
        "WORD_OPTION",
        "WORD_LIMITER",
        "WORD_STRING",
        "WORD_ERROR",
        "WORD_WTF",
    };
    if (current->type >= 0 && current->type < 28) {
        return Token_Class[current->type];
    } else {
        return "NULL";
    }
}

t_ast* createNode(enum s_type type, char* value) {
    t_ast* node = (t_ast*)malloc(sizeof(t_ast));
    if (!node)
        return NULL;
    node->type = type;
    node->value = strdup(value);
    if (!node->value) {
        free(node);
        return NULL;
    }
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

t_ast* handleOpenParenthesis(t_token **tokens, t_ast* current) {
    *tokens = (*tokens)->next;
    t_ast* subTree = parseSubexpression(tokens);
    if (current) {
        current->right = subTree;
        subTree->parent = current;
        current = subTree;
    } else {
        current = subTree;
    }
    return current;
}

t_ast* handleCloseParenthesis(t_token **tokens, t_ast* root) {
    *tokens = (*tokens)->next;
    return root;
}

t_ast* handleAndOr(t_token **tokens, t_ast* root) {
    t_ast* newNode = createNode((*tokens)->type, (*tokens)->value);
    newNode->left = root;
    if (root) root->parent = newNode;
    root = newNode;
    *tokens = (*tokens)->next;
    return root;
}

t_ast* handlePipe(t_token **tokens, t_ast* root) {
    t_ast* newNode = createNode((*tokens)->type, (*tokens)->value);
    newNode->left = root;
    if (root) root->parent = newNode;
    root = newNode;
    *tokens = (*tokens)->next;
    return root;
}

// (cat <<  LIM  && (echo hello && (echo world || echo test))) && echo oui | echo non > output.txt
/*
	<<
		LIM
	current == cat
		cat
			<<
				LIM
*/
t_ast* handleRedirect(t_token **tokens, t_ast* current) {
	t_ast* newNode = createNode((*tokens)->type, (*tokens)->value);
    *tokens = (*tokens)->next;
    if (*tokens) {
        t_ast* fileNode = createNode((*tokens)->type, (*tokens)->value);
        newNode->right = fileNode;
        fileNode->parent = newNode;
        *tokens = (*tokens)->next;
    }
    if (current) { //cat
        if (current->right) {
            t_ast* temp = current->right;
            while (temp->right) temp = temp->right;
            temp->right = newNode;
            newNode->parent = temp;
        } else {
			
            current->right = newNode;
			newNode->parent = current;
			char *temp;
			int	temp_type;
			temp = ft_strdup(current->value);
			temp_type = current->type;
			current->value = ft_strdup(newNode->value);
			current->type = newNode->type;
			newNode->value = ft_strdup(temp);
			newNode->type = temp_type;
			
        }
    } else {
        current = newNode;
    }
    return current;
}

t_ast* handleBuiltinCmdQuotes(t_token **tokens, t_ast* current) {
    t_ast* newNode = createNode((*tokens)->type, (*tokens)->value);
    if (current) {
        current->right = newNode;
        newNode->parent = current;
        current = newNode;
    } else {
        current = newNode;
    }
    *tokens = (*tokens)->next;
    return current;
}

t_ast* handleOption(t_token **tokens, t_ast* current) {
    t_ast* newNode = createNode((*tokens)->type, (*tokens)->value);
    if (current && current->type == WORD_BUILTIN) {
        if (current->right) {
            t_ast* temp = current->right;
            while (temp->right) temp = temp->right;
            temp->right = newNode;
            newNode->parent = temp;
        } else {
            current->right = newNode;
            newNode->parent = current;
        }
    } else {
        if (current) {
            current->right = newNode;
            newNode->parent = current;
            current = newNode;
        } else {
            current = newNode;
        }
    }
    *tokens = (*tokens)->next;
    return current;
}

t_ast* parseExpression(t_token **tokens) {
    t_ast *root = NULL;
    t_ast *current = NULL;

    while (*tokens) {
        printf("token = %s\n", (*tokens)->value);
        if ((*tokens)->type == TOKEN_OPENPAR) {
            current = handleOpenParenthesis(tokens, current);
            if (!root) root = current;
        } else if ((*tokens)->type == TOKEN_CLOSEPAR) {
            return handleCloseParenthesis(tokens, root);
        } else if ((*tokens)->type == TOKEN_APPENDOUT || (*tokens)->type == TOKEN_REDIRECTIN || (*tokens)->type == TOKEN_REDIRECTOUT || (*tokens)->type == TOKEN_HEREDOC) {
            current = handleRedirect(tokens, current);
            if (!root) root = current;
        } else if ((*tokens)->type == TOKEN_PIPE) {
            root = handlePipe(tokens, root);
            current = root;
        } else if ((*tokens)->type == TOKEN_AND || (*tokens)->type == TOKEN_OR) {
            root = handleAndOr(tokens, root);
            current = root;
        } else if ((*tokens)->type == WORD_BUILTIN || (*tokens)->type == WORD_CMD || (*tokens)->type == TOKEN_DQUOTES || (*tokens)->type == TOKEN_SQUOTES) {
            current = handleBuiltinCmdQuotes(tokens, current);
            if (!root) root = current;
        } else if ((*tokens)->type == WORD_OPTION) {
            current = handleOption(tokens, current);
            if (!root) root = current;
        } else if ((*tokens)->type == TOKEN_WHITESPACE) {
            (*tokens) = (*tokens)->next;
        } else {
            return NULL;
        }
    }
    return root;
}

t_ast* parseSubexpression(t_token **tokens) {
    return parseExpression(tokens);
}
































/* enum s_type{
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
typedef struct s_ast {
	enum s_type type;
	char		*value;
	struct s_ast *left;
	struct s_ast *right;
	struct s_ast *parent;
}				t_ast;

//

void printAST(t_ast* node, int level)
{
    if (node == NULL) return;
    
    // Print the right subtree first (with increased level)
    if (node->right) {
        printAST(node->right, level + 1);
    }

    // Print current node with proper indentation
    for (int i = 0; i < level; i++) printf("    ");
    printf("%s: %s\n",getAST_Class(node), node->value);

    // Print the left subtree (with increased level)
    if (node->left) {
        printAST(node->left, level + 1);
    }
}

const char* getAST_Class(t_ast *current)
{
	const char* Token_Class[] = {
    	"NOT_DEFINE",
		"TOKEN_WORD",
		"TOKEN_DQUOTES",
		"TOKEN_SQUOTES",
		"TOKEN_AND",
		"TOKEN_OR",
		"TOKEN_PIPE",
		"TOKEN_REDIRECTIN",
		"TOKEN_REDIRECTOUT",
		"TOKEN_HEREDOC",
		"TOKEN_APPENDOUT",
		"TOKEN_LIMITER",
		"TOKEN_OPENPAR",
		"TOKEN_CLOSEPAR",
		"TOKEN_WHITESPACE",
		"TOKEN_ENV",
		"TOKEN_WILDCARD",
		"TOKEN_COUNT",
		"WORD_FILEIN",
		"WORD_FILEOUT",
		"WORD_FILEOUT_APPEND",
		"WORD_BUILTIN",
		"WORD_ABSPATH",
		"WORD_CMD",
		"WORD_OPTION",
		"WORD_LIMITER",
		"WORD_STRING",
		"WORD_ERROR",
		"WORD_WTF",
	};
    if (current->type >= 0 && current->type < 28) {
        return Token_Class[current->type];
    } else {
        return "NULL";
    }
	
}

t_ast	*createNode(enum s_type type, char* value)
{
	t_ast* node;
	
	node = (t_ast*)malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->value = ft_strdup(value); // a remplacer par ft_strdup
	if (!node->value)
		return (NULL);
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;
	return node;
}

void addRightChild(t_ast* current, t_ast* newNode) {
    if (current == NULL || newNode == NULL) {
        return;
    }
    
    current->right = newNode;
    newNode->parent = current;
    current = newNode; // Cela n'est plus nécessaire ici, car current devrait déjà pointer vers newNode
}

t_ast	*handleOpenParenthesis(t_token **tokens, t_ast* current)
{
	//ajouter toke next ?
	*tokens = (*tokens)->next;
	t_ast* subTree = parseSubexpression(tokens);
	if (current)
	{
		current->right = subTree;
		subTree->parent = current;
		current = subTree;
	}
	else
		current = subTree;
	return current;
}

t_ast	*handleCloseParenthesis(t_token **tokens, t_ast* root)
{
	//rajouter token next ?
	*tokens = (*tokens)->next;
	return (root);
}

t_ast	*handleAndOr(t_token **tokens, t_ast* root)
{
	t_ast	*newNode;

	newNode = createNode((*tokens)->type, (*tokens)->value);
    newNode->left = root;
    if (root) root->parent = newNode;
    root = newNode;
	*tokens = (*tokens)->next;
    return root;
}

t_ast	*handleBuiltinCmdQuotes(t_token **tokens, t_ast* current)
{
	t_ast* newNode;
	
	newNode = createNode((*tokens)->type, (*tokens)->value);
	if (current)
	{
		current->right = newNode;
		newNode->parent = current;
		current = newNode;
	}
	else
		current = newNode;
	*tokens = (*tokens)->next;
	return current;
}

t_ast	*handleOption(t_token **tokens, t_ast* current)
{
	t_ast*	newNode;

	newNode = createNode((*tokens)->type, (*tokens)->value);
	if (current && current->type == WORD_BUILTIN)
	{
		if (current->right)
		{
			t_ast* temp = current->right;
			while (temp->right) temp = temp->right;
			temp->right = newNode;
			newNode->parent = temp;
		}
		else
		{
			current->right = newNode;
			newNode->parent = current;
		}
	}
	else
	{
		if (current)
		{
			current->right = newNode;
			newNode->parent = current;
			current = newNode;
		}
		else
			current = newNode;
	}
	*tokens = (*tokens)->next;
	//ajouter token next ?
	return current;
}


//definir root pour factoriser
t_ast* parseExpression(t_token **tokens)
{
	t_ast *root;
	t_ast *current;

	root = NULL;
	current = NULL;
	while (*tokens) //echo //test
	{
		printf("token = %s\n", (*tokens)->value);
		// Traiter le jeton courant
		if ((*tokens)->type == TOKEN_OPENPAR)
		{
			current = handleOpenParenthesis(tokens, current);
			if (!root)
				root = current;
		}
		else if ((*tokens)->type == TOKEN_CLOSEPAR)
			return handleCloseParenthesis(tokens, root);
		else if ((*tokens)->type == TOKEN_AND || (*tokens)->type == TOKEN_OR)
		{
			root = handleAndOr(tokens, root);
			current = root;
		}
		else if ((*tokens)->type == WORD_BUILTIN || (*tokens)->type == WORD_CMD || (*tokens)->type == TOKEN_DQUOTES || (*tokens)->type == TOKEN_SQUOTES)
		{
			current = handleBuiltinCmdQuotes(tokens, current);
			if (!root) {
				root = current;
			}
		}
		else if ((*tokens)->type == WORD_OPTION)
		{
			current = handleOption(tokens, current);
			if (!root) {
				root = current;
			}
		}
		else if ((*tokens)->type == TOKEN_WHITESPACE)
            (*tokens) = (*tokens)->next;
        else
            return NULL;
	}
	return (root);
}

t_ast	*parseSubexpression(t_token **tokens)
{
	return parseExpression(tokens);
} */

// ( echo test && echo yo ) && ( echo coucou && ( echo prout && ( echo ok ) ) ) && ls | grep > output.txt



