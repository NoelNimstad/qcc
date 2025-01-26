#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"
#include "modifiers.h"

// case NODE_TYPE_INT:
// case NODE_TYPE_FLOAT:
// case NODE_TYPE_DOUBLE:
// case NODE_TYPE_VOID:
// case NODE_TYPE_uint8:
// case NODE_TYPE_sint8:
// case NODE_TYPE_uint16:
// case NODE_TYPE_sint16:
// case NODE_TYPE_uint32:
// case NODE_TYPE_sint32:
// case NODE_TYPE_uint64:
// case NODE_TYPE_sint64:
// case NODE_TYPE_double64:
// case NODE_TYPE_double128:

char *parseNode(Node *node)
{
    if (node == NULL) return strdup("");

    switch (node->type)
    {
        case NODE_VARIABLE_DECLARATION:
        {
            char *typeString = strdup(
                (node->left->type == NODE_TYPE_INT)       ? "int"                    :
                (node->left->type == NODE_TYPE_FLOAT)     ? "float"                  :
                (node->left->type == NODE_TYPE_DOUBLE)    ? "double"                 :
                (node->left->type == NODE_TYPE_VOID)      ? "void"                   :
                (node->left->type == NODE_TYPE_uint8)     ? "unsigned char"          :
                (node->left->type == NODE_TYPE_sint8)     ? "signed char"            :
                (node->left->type == NODE_TYPE_uint16)    ? "unsigned short int"     :
                (node->left->type == NODE_TYPE_sint16)    ? "signed short int"       :
                (node->left->type == NODE_TYPE_uint32)    ? "unsigned long int"      :
                (node->left->type == NODE_TYPE_sint32)    ? "signed long int"        :
                (node->left->type == NODE_TYPE_uint64)    ? "unsigned long long int" :
                (node->left->type == NODE_TYPE_sint64)    ? "signed long long int"   :
                (node->left->type == NODE_TYPE_double64)  ? "double"                 :
				(node->left->type == NODE_TYPE_double128) ? "long double"            :
                                                            "/*error*/");
                                                            
			unsigned char mutable = (node->modifiers & MODIFIER_MUTABLE) == 0;
            char *rightString = parseNode(node->right);

            size_t resultSize = strlen(typeString) + strlen(rightString) + 2 + (mutable ? 6 : 0);
            char *result = (char *)malloc(resultSize);
            snprintf(result, resultSize, "%s%s %s", (mutable ? "const ": ""), typeString, rightString);

            free(typeString);
            free(rightString);
            return result;
        }
        case NODE_VALUE_INT:
        {
            char buffer[32];
            snprintf(buffer, sizeof(buffer), "%d", node->value.int_value);
            return strdup(buffer);
        }
        case NODE_VALUE_FLOAT:
        {
            char buffer[32];
            snprintf(buffer, sizeof(buffer), "%ff", node->value.float_value);
            return strdup(buffer);
        }
        case NODE_IDENTIFIER:
        {
            return strdup(node->value.string_value);
        }
        case NODE_OPERATOR_ASSIGN:
        {
            char *leftString = parseNode(node->left);
            char *rightString = parseNode(node->right);

            size_t resultSize = strlen(leftString) + strlen(rightString) + 4;
            char *result = (char *)malloc(resultSize);
            snprintf(result, resultSize, "%s = %s", leftString, rightString);

            free(leftString);
            free(rightString);
            return result;
        }
        case NODE_OPERATOR_PLUS:
        case NODE_OPERATOR_MINUS:
        case NODE_OPERATOR_MULTIPLY:
        case NODE_OPERATOR_DIVIDE:
        {
            char *leftString = parseNode(node->left);
            char *rightString = parseNode(node->right);

            const char *op =
                (node->type == NODE_OPERATOR_PLUS)     ? "+" :
                (node->type == NODE_OPERATOR_MINUS)    ? "-" :
                (node->type == NODE_OPERATOR_MULTIPLY) ? "*" : "/";

            size_t resultSize = strlen(leftString) + strlen(rightString) + 4;
            char *result = (char *)malloc(resultSize);
            snprintf(result, resultSize, "%s %s %s", leftString, op, rightString);

            free(leftString);
            free(rightString);
            return result;
        }
        case NODE_OPERATOR_SEMI_COLON:
        {
            return strdup("; ");
        }
        case NODE_NEW_LINE:
        {
            return strdup("\n");
        }
        case NODE_HEAD:
        {
            return strdup("");
        }
        case NODE_END_OF_FILE:
		{
			return strdup("\n");
		}
        default:
        {
            return strdup("/*error*/");
        }
    }
}

char *parseTree(Node *head)
{
    if (head == NULL) return strdup("");

    Node *current = head;
    char *result = strdup("");

    while (current != NULL && current->type != NODE_END_OF_FILE)
    {
        char *nodeString = parseNode(current);

        // Resize result and append the parsed node string
        size_t newSize = strlen(result) + strlen(nodeString) + 2;
        result = (char *)realloc(result, newSize);
        strcat(result, nodeString);

        // Add a space or newline after nodes, if necessary
        if (current->type != NODE_OPERATOR_SEMI_COLON &&
            current->type != NODE_NEW_LINE &&
            current->type != NODE_END_OF_FILE)
        {
            strcat(result, "");
        }

        free(nodeString);
        current = current->next;
    }

    return result;
}

#endif // PARSE_H