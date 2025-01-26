#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>

#include "tokens.h"
#include "modifiers.h"

typedef enum 
{
	// types
	NODE_TYPE_uint8,
	NODE_TYPE_sint8,
	NODE_TYPE_uint16,
	NODE_TYPE_sint16,
	NODE_TYPE_uint32,
	NODE_TYPE_sint32,
	NODE_TYPE_uint64,
	NODE_TYPE_sint64,
	NODE_TYPE_INT,
	NODE_TYPE_FLOAT,
	NODE_TYPE_double64,
	NODE_TYPE_double128,
	NODE_TYPE_DOUBLE,
	NODE_TYPE_STRING,
	NODE_TYPE_VOID,

	// values
	NODE_VALUE_INT,
	NODE_VALUE_FLOAT,

	// operators
    NODE_OPERATOR_ASSIGN,
    NODE_OPERATOR_PLUS,
    NODE_OPERATOR_MINUS,
    NODE_OPERATOR_MULTIPLY,
    NODE_OPERATOR_DIVIDE,
    NODE_OPERATOR_SEMI_COLON,

	// variables
	NODE_IDENTIFIER,
    NODE_VARIABLE_DECLARATION,
    NODE_FUNCTION_DECLARATION,
    NODE_FUNCTION_PARAMETER,

	// miscelanious
	NODE_HEAD,
	NODE_END_OF_FILE,
	NODE_NEW_LINE,
	NODE_ERROR,
} NodeType;

typedef struct Node
{
	NodeType type;
    unsigned int modifiers;

	struct Node *left;
	struct Node *right;

	union
	{
		char *string_value;
		int int_value;
		float float_value;
		double double_value;
	} value;

	struct Node *previous;
	struct Node *next;
} Node;

void movePreviousToLeft(Node *node)
{
    if(!node->previous) return;

	node->left = node->previous;
	node->previous = node->previous->previous;
	if(node->previous)
	{
		node->previous->next = node;
	}
	if(node->left)
	{
		node->left->previous = NULL;
		node->left->next = NULL;
	}
}

unsigned char tokenIsType(Token **inputToken)
{
    unsigned char isType = (((*inputToken)->type == TOKEN_TYPE_INT)       ? 1 :
                            ((*inputToken)->type == TOKEN_TYPE_uint8)     ? 1 :
                            ((*inputToken)->type == TOKEN_TYPE_sint8)     ? 1 :
                            ((*inputToken)->type == TOKEN_TYPE_uint16)    ? 1 :
                            ((*inputToken)->type == TOKEN_TYPE_sint16)    ? 1 :
                            ((*inputToken)->type == TOKEN_TYPE_uint32)    ? 1 :
                            ((*inputToken)->type == TOKEN_TYPE_sint32)    ? 1 :
                            ((*inputToken)->type == TOKEN_TYPE_uint64)    ? 1 :
                            ((*inputToken)->type == TOKEN_TYPE_sint64)    ? 1 :
                            ((*inputToken)->type == TOKEN_TYPE_FLOAT)     ? 1 :
                            ((*inputToken)->type == TOKEN_TYPE_DOUBLE)    ? 1 :
                            ((*inputToken)->type == TOKEN_TYPE_double64)  ? 1 :
                            ((*inputToken)->type == TOKEN_TYPE_double128) ? 1 :
                                                                            0);

    return isType;
}

NodeType convertTokenTypeToNodeType(Token **inputToken)
{
    NodeType nodeType = (((*inputToken)->type == TOKEN_TYPE_INT)       ? NODE_TYPE_INT       :
                         ((*inputToken)->type == TOKEN_TYPE_uint8)     ? NODE_TYPE_uint8     :
                         ((*inputToken)->type == TOKEN_TYPE_sint8)     ? NODE_TYPE_sint8     :
                         ((*inputToken)->type == TOKEN_TYPE_uint16)    ? NODE_TYPE_uint16    :
                         ((*inputToken)->type == TOKEN_TYPE_sint16)    ? NODE_TYPE_sint16    :
                         ((*inputToken)->type == TOKEN_TYPE_uint32)    ? NODE_TYPE_uint32    :
                         ((*inputToken)->type == TOKEN_TYPE_sint32)    ? NODE_TYPE_sint32    :
                         ((*inputToken)->type == TOKEN_TYPE_uint64)    ? NODE_TYPE_uint64    :
                         ((*inputToken)->type == TOKEN_TYPE_sint64)    ? NODE_TYPE_sint64    :
                         ((*inputToken)->type == TOKEN_TYPE_FLOAT)     ? NODE_TYPE_FLOAT     :
                         ((*inputToken)->type == TOKEN_TYPE_DOUBLE)    ? NODE_TYPE_DOUBLE    :
                         ((*inputToken)->type == TOKEN_TYPE_double64)  ? NODE_TYPE_double64  :
                         ((*inputToken)->type == TOKEN_TYPE_double128) ? NODE_TYPE_double128 :
                                                                         NODE_ERROR);

    return nodeType;
}

Node *parseExpression(Token **inputToken);
Node *parseVariableDeclaration(Token **inputToken);
Node *parseFunctionDeclaration(Token **inputToken);

unsigned int currentModifiers;
Node *generateNodeAtPosition(Node *position, Token **inputToken, Node *previous)
{
    Token *currentToken = *inputToken;

    Node *currentNode = (Node *)malloc(sizeof(Node));
    currentNode->type = NODE_ERROR;
    currentNode->left = NULL;
    currentNode->right = NULL;
    currentNode->previous = NULL;
    currentNode->next = NULL;
    currentNode->modifiers = MODIFIER_NONE;

    position = currentNode;
    if(previous != NULL)
    {
        currentNode->previous = previous;
        previous->next = currentNode;
    }

    while(currentToken->type == TOKEN_MODIFIER)
    {
        if(strcmp(currentToken->value.string_value, "mutable"))
        {
            currentModifiers |= MODIFIER_MUTABLE;
            (*inputToken)++;
            currentToken = *inputToken;
        } else if(strcmp(currentToken->value.string_value, "mut"))
        {
            currentModifiers |= MODIFIER_MUTABLE;
            (*inputToken)++;
            currentToken = *inputToken;
        }
    }

    switch(currentToken->type)
    {
        case TOKEN_TYPE_INT:
        case TOKEN_TYPE_FLOAT:
        case TOKEN_TYPE_DOUBLE:
        case TOKEN_TYPE_uint8:
        case TOKEN_TYPE_sint8:
        case TOKEN_TYPE_uint16:
        case TOKEN_TYPE_sint16:
        case TOKEN_TYPE_uint32:
        case TOKEN_TYPE_sint32:
        case TOKEN_TYPE_uint64:
        case TOKEN_TYPE_sint64:
        case TOKEN_TYPE_double64:
        case TOKEN_TYPE_double128:
            currentNode->left = (Node *)malloc(sizeof(Node));
            currentNode->left->type = convertTokenTypeToNodeType(inputToken);
            (*inputToken)++; currentToken = (*inputToken); // consume token

            if(currentToken->type == TOKEN_IDENTIFIER)
            {
                (*inputToken)++;
                if((*inputToken)->type == TOKEN_LEFT_ROUND_BRACKET)
                {
                    (*inputToken)--;
                    currentNode->type = NODE_FUNCTION_DECLARATION;
                    currentNode->modifiers = currentModifiers;
                    currentNode->right = parseFunctionDeclaration(inputToken);
                    currentModifiers = MODIFIER_NONE;
                    return currentNode;
                } else
                {
                    (*inputToken)--;
                    currentNode->type = NODE_VARIABLE_DECLARATION;
                    currentNode->right = parseVariableDeclaration(inputToken);
                    currentNode->modifiers = currentModifiers;
                    currentModifiers = MODIFIER_NONE;
                    return currentNode;
                }
            }

            return currentNode;
        case TOKEN_VALUE_INT:
            currentNode->type = NODE_VALUE_INT;
            currentNode->value.int_value = currentToken->value.int_value;
            break;
        case TOKEN_VALUE_FLOAT:
            currentNode->type = NODE_VALUE_FLOAT;
            currentNode->value.float_value = currentToken->value.float_value;
            break;
        case TOKEN_IDENTIFIER:
            currentNode->type = NODE_IDENTIFIER;
            currentNode->value.string_value = strdup(currentToken->value.string_value);
            break;
        case TOKEN_OPERATOR_ASSIGN:
            currentNode->type = NODE_OPERATOR_ASSIGN;
            movePreviousToLeft(currentNode);

            (*inputToken)++;
            currentNode->right = parseExpression(inputToken);
            return currentNode;
        case TOKEN_OPERATOR_PLUS:
        case TOKEN_OPERATOR_MINUS:
        case TOKEN_OPERATOR_MULTIPLY:
        case TOKEN_OPERATOR_DIVIDE:
            currentNode->type = (currentToken->type == TOKEN_OPERATOR_PLUS)     ? NODE_OPERATOR_PLUS     :
                                (currentToken->type == TOKEN_OPERATOR_MINUS)    ? NODE_OPERATOR_MINUS    :
                                (currentToken->type == TOKEN_OPERATOR_MULTIPLY) ? NODE_OPERATOR_MULTIPLY :
                                                                                  NODE_OPERATOR_DIVIDE;

            movePreviousToLeft(currentNode);
            (*inputToken)++;
            currentNode->right = parseExpression(inputToken);
            return currentNode;
        case TOKEN_LEFT_ROUND_BRACKET:
            (*inputToken)++;
            currentNode = parseExpression(inputToken);
            if((*inputToken)->type != TOKEN_RIGHT_ROUND_BRACKET)
            {
                printf("Error: Missing closing parenthesis\n");
                currentNode->type = NODE_ERROR;
            } else
            {
                (*inputToken)++;
            }
            return currentNode;
        case TOKEN_END_OF_FILE:
            currentNode->type = NODE_END_OF_FILE;
            break;
        case TOKEN_OPERATOR_SEMI_COLON:
            currentNode->type = NODE_OPERATOR_SEMI_COLON;
            break;
        default:
            printf("Unexpected token type: %d\n", currentToken->type);
            currentNode->type = NODE_ERROR;
            break;
    }

    return currentNode;
}

Node *parseVariableDeclaration(Token **inputToken)
{
    Token *currentToken = *inputToken;

    if(currentToken->type != TOKEN_IDENTIFIER)
    {
        printf("Error: Expected variable identifier after type\n");
        return NULL;
    }

    Node *identifierNode = (Node *)malloc(sizeof(Node));
    identifierNode->type = NODE_IDENTIFIER;
    identifierNode->left = NULL;
    identifierNode->right = NULL;
    identifierNode->previous = NULL;
    identifierNode->next = NULL;
    identifierNode->modifiers = MODIFIER_NONE;
    identifierNode->value.string_value = strdup(currentToken->value.string_value);

    (*inputToken)++;

    if((*inputToken)->type == TOKEN_OPERATOR_ASSIGN)
    {
        Node *assignNode = (Node *)malloc(sizeof(Node));
        assignNode->type = NODE_OPERATOR_ASSIGN;
        assignNode->left = identifierNode;

        (*inputToken)++;
        assignNode->right = parseExpression(inputToken);

        return assignNode;
    }

    return identifierNode;
}

Node *parseFunctionDeclaration(Token **inputToken)
{
    Token *currentToken = *inputToken;

    if(currentToken->type != TOKEN_IDENTIFIER)
    {
        printf("Error: Expected function name after type\n");
        return NULL;
    }

    Node *identifierNode = (Node *)malloc(sizeof(Node));
    identifierNode->type = NODE_IDENTIFIER;
    identifierNode->left = NULL;
    identifierNode->right = NULL;
    identifierNode->previous = NULL;
    identifierNode->next = NULL;
    identifierNode->modifiers = MODIFIER_NONE;
    identifierNode->value.string_value = strdup(currentToken->value.string_value);

    (*inputToken)++;
    if((*inputToken)->type != TOKEN_LEFT_ROUND_BRACKET)
    {
        printf("Error: Expected '(' after function name\n");
    }

    (*inputToken)++;
    while((*inputToken)->type != TOKEN_RIGHT_ROUND_BRACKET)
    {
        if(tokenIsType(inputToken))
        {
            printf("Banana!\n");
        }

        (*inputToken)++;
    }

    return identifierNode;
}

Node *parseExpression(Token **inputToken)
{
    Token *currentToken = *inputToken;
    Node *leftNode = generateNodeAtPosition(NULL, inputToken, NULL);

    while(currentToken->type == TOKEN_OPERATOR_PLUS     ||
          currentToken->type == TOKEN_OPERATOR_MINUS    ||
          currentToken->type == TOKEN_OPERATOR_MULTIPLY ||
          currentToken->type == TOKEN_OPERATOR_DIVIDE)
    {
        Node *operatorNode = generateNodeAtPosition(NULL, inputToken, leftNode);
        operatorNode->left = leftNode;
        leftNode = operatorNode;

        currentToken = *inputToken;
    }

    return leftNode;
}

Node *generateNodeTree(Token *tokens)
{
	Node *head = (Node *)malloc(sizeof(Node));
	head->type = NODE_HEAD;

	Node *currentNode = head;
	currentNode->left = NULL;
	currentNode->right = NULL;
	currentNode->previous = NULL;
	currentNode->next = NULL;
    currentNode->modifiers = MODIFIER_NONE;

	for (Token *currentToken = tokens; currentToken->type != TOKEN_END_OF_FILE;)
	{
		if(currentToken->type == TOKEN_ERROR)
		{
			currentToken++;
			continue;
		}

		currentNode = generateNodeAtPosition(currentNode->next, &currentToken, currentNode);
		currentToken++;
	}

	Node *EoF = (Node *)malloc(sizeof(Node));
	EoF->previous = currentNode;
	currentNode->next = EoF;
	EoF->type = NODE_END_OF_FILE;
	EoF->left = NULL;
	EoF->right = NULL;

	return head;
}

void freeNodeTree(Node *node)
{
    if(node == NULL) return;

    freeNodeTree(node->left);
    freeNodeTree(node->right);

    if((node->type == NODE_IDENTIFIER || node->type == NODE_TYPE_STRING) &&
        node->value.string_value != NULL)
	{
        free(node->value.string_value);
    }

    free(node);
}

#endif