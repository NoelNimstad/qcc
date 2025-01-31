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

unsigned int getModifierValueFromTokenString(Token **inputToken)
{
    if(strcmp((*inputToken)->value.string_value, "mut")     == 0
    || strcmp((*inputToken)->value.string_value, "mutable") == 0)
    {
        return MODIFIER_MUTABLE;
    } else if(strcmp((*inputToken)->value.string_value, "const")    == 0
           || strcmp((*inputToken)->value.string_value, "constant") == 0)
    {
        return MODIFIER_CONST;
    }

    return MODIFIER_NONE;
}

Node *newEmptyNode()
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NODE_ERROR;
    node->left = NULL;
    node->right = NULL;
    node->previous = NULL;
    node->next = NULL;
    node->modifiers = MODIFIER_NONE;

    return node;
}

Node *parseExpression(Token **inputToken);
Node *parseVariableDeclaration(Token **inputToken);
Node *parseFunctionDeclaration(Token **inputToken);
void freeNodeTree(Node *node);

unsigned int currentModifiers;
Node *generateNodeAtPosition(Node *position, Token **inputToken, Node *previous)
{
    Node *currentNode = newEmptyNode();

    position = currentNode;
    if(previous != NULL)
    {
        currentNode->previous = previous;
        previous->next = currentNode;
    }

    while((*inputToken)->type == TOKEN_MODIFIER)
    {
        printf("%s\n", (*inputToken)->value.string_value);
        currentModifiers |= getModifierValueFromTokenString(inputToken);
        printf("m%d\n", getModifierValueFromTokenString(inputToken));
        (*inputToken)++;
    }

    switch((*inputToken)->type)
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
            (*inputToken)++;

            if((*inputToken)->type == TOKEN_IDENTIFIER)
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
            currentNode->value.int_value = (*inputToken)->value.int_value;
            break;
        case TOKEN_VALUE_FLOAT:
            currentNode->type = NODE_VALUE_FLOAT;
            currentNode->value.float_value = (*inputToken)->value.float_value;
            break;
        case TOKEN_IDENTIFIER:
            currentNode->type = NODE_IDENTIFIER;
            currentNode->value.string_value = strdup((*inputToken)->value.string_value);
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
            currentNode->type = ((*inputToken)->type == TOKEN_OPERATOR_PLUS)     ? NODE_OPERATOR_PLUS     :
                                ((*inputToken)->type == TOKEN_OPERATOR_MINUS)    ? NODE_OPERATOR_MINUS    :
                                ((*inputToken)->type == TOKEN_OPERATOR_MULTIPLY) ? NODE_OPERATOR_MULTIPLY :
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
            printf("Unexpected token type: %d\n", (*inputToken)->type);
            currentNode->type = NODE_ERROR;
            break;
    }

    return currentNode;
}

Node *parseVariableDeclaration(Token **inputToken)
{
    if((*inputToken)->type != TOKEN_IDENTIFIER)
    {
        printf("Error: Expected variable identifier after type\n");
        return NULL;
    }

    Node *identifierNode = newEmptyNode();
    identifierNode->type = NODE_IDENTIFIER;
    identifierNode->value.string_value = strdup((*inputToken)->value.string_value);

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
    if((*inputToken)->type != TOKEN_IDENTIFIER)
    {
        printf("Error: Expected function name after type\n");
        return NULL;
    }

    Node *identifierNode = newEmptyNode();
    identifierNode->type = NODE_IDENTIFIER;
    identifierNode->value.string_value = strdup((*inputToken)->value.string_value);

    (*inputToken)++;
    if((*inputToken)->type != TOKEN_LEFT_ROUND_BRACKET)
    {
        printf("Error: Expected '(' after function name\n");
    }

    Node *parameterList = NULL;
    Node *lastParameter = NULL;

    (*inputToken)++;
    while((*inputToken)->type != TOKEN_RIGHT_ROUND_BRACKET)
    {
        if((*inputToken)->type == TOKEN_MODIFIER)
        {
            currentModifiers |= getModifierValueFromTokenString(inputToken);
            printf("%d\n", getModifierValueFromTokenString(inputToken));
            (*inputToken)++;
            continue;
        }

        if(tokenIsType(inputToken))
        {
            Node *parameter = newEmptyNode();
            parameter->type = NODE_FUNCTION_PARAMETER;
            printf("%d\n", currentModifiers);
            parameter->modifiers = currentModifiers;
            currentModifiers = MODIFIER_NONE;

            parameter->left = (Node *)malloc(sizeof(Node));
            parameter->left->type = convertTokenTypeToNodeType(inputToken);
            parameter->left->left = NULL;
            parameter->left->right = NULL;

            (*inputToken)++;
            if((*inputToken)->type == TOKEN_IDENTIFIER)
            {
                parameter->right = newEmptyNode();
                parameter->right->type = NODE_IDENTIFIER;

                parameter->value.string_value = strdup((*inputToken)->value.string_value);
                (*inputToken)++;
            } else
            {
                printf("Error: Expected parameter name\n");
                freeNodeTree(parameter);
                freeNodeTree(identifierNode);
                return NULL;
            }

            if(parameterList == NULL)
            {
                parameterList = parameter;
            }
            else
            {
                lastParameter->next = parameter;
                parameter->previous = lastParameter;
            }
            lastParameter = parameter;

            if((*inputToken)->type == TOKEN_COMMA)
            {
                (*inputToken)++;
            } else if((*inputToken)->type != TOKEN_RIGHT_ROUND_BRACKET)
            {
                printf("Error: Unexpected token in parameter list: %d\n", (*inputToken)->type);
                freeNodeTree(identifierNode);
                return NULL;
            }
        } else 
        {
            printf("Error: Unexpected token in parameter list: %d\n", (*inputToken)->type);
        }
    }

    return identifierNode;
}

Node *parseExpression(Token **inputToken)
{
    Node *leftNode = generateNodeAtPosition(NULL, inputToken, NULL);

    while((*inputToken)->type == TOKEN_OPERATOR_PLUS     ||
          (*inputToken)->type == TOKEN_OPERATOR_MINUS    ||
          (*inputToken)->type == TOKEN_OPERATOR_MULTIPLY ||
          (*inputToken)->type == TOKEN_OPERATOR_DIVIDE)
    {
        Node *operatorNode = generateNodeAtPosition(NULL, inputToken, leftNode);
        operatorNode->left = leftNode;
        leftNode = operatorNode;
    }

    return leftNode;
}

Node *generateNodeTree(Token *tokens)
{
	Node *head = newEmptyNode();
	head->type = NODE_HEAD;
	Node *currentNode = head;

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