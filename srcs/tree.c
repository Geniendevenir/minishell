/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 11:50:03 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/03 12:07:33 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

typedef enum {
    COMMAND,
    AND,
    OR,
    PIPE,
    REDIRECT_IN,
    REDIRECT_OUT,
    REDIRECT_APPEND,
	HERE_DOC,
} NodeType;


typedef struct Node
{
    NodeType type;
    struct Node *left;
    struct Node *right;
    struct Node *parent;
    
} Node;

Node* create_node(NodeType type)
{
    Node *node = (Node*)malloc(sizeof(Node));
    if (!node)
        return NULL;
    node->type = type;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}