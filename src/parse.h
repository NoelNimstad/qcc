#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"
#include "modifiers.h"

char *nodeTypeToString(Node *node)
{
    switch(node->type)
    {
    case NODE_TYPE_INT:
        return "int";
        break;
    case NODE_TYPE_FLOAT:
        return "float";
        break;
    case NODE_TYPE_DOUBLE:
        return "double";
        break;
    case NODE_TYPE_VOID:
        return "void";
        break;
    case NODE_TYPE_uint8:
        return "unsigned char";
        break;
    case NODE_TYPE_sint8:
        return "signed char";
        break;
    case NODE_TYPE_uint16:
        return "unsigned short int";
        break;
    case NODE_TYPE_sint16:
        return "signed short int";
        break;
    case NODE_TYPE_uint32:
        return "unsigned long int";
        break;
    case NODE_TYPE_sint32:
        return "signed long int";
        break;
    case NODE_TYPE_uint64:
        return "unsigned long long int";
        break;
    case NODE_TYPE_sint64:
        return "signed long long int";
        break;
    case NODE_TYPE_double64:
        return "double";
        break;
    case NODE_TYPE_double128:
        return "long double";
        break;
    default:
        return "/*error*/";
        break;
    }
}

char *parseNode(Node *node)
{
    if (node == NULL) return strdup("");

    switch (node->type)
    {
        case NODE_VARIABLE_DECLARATION:
        {
            char *typeString = nodeTypeToString(node->left);      
			unsigned char mutable = (node->modifiers & MODIFIER_MUTABLE) == 0; // for modifiers @mut / @mutable
            char *rightString = parseNode(node->right);

            size_t resultSize = strlen(typeString) + strlen(rightString) + 2 + (mutable ? 6 : 0);
            char *result = (char *)malloc(resultSize);
            snprintf(result, resultSize, "%s%s %s", (mutable ? "const ": ""), typeString, rightString);

            free(rightString);
            return result;
        }
        case NODE_FUNCTION_DECLARATION:
        {
            char *typeString = nodeTypeToString(node->left);
            unsigned char constant = (node->modifiers & MODIFIER_CONST) != 0; // for modifiers @const / @constant
            char *identifierString = parseNode(node->right);

            char *args = strdup("void");
            unsigned char hasArguments = node->right->left != NULL;
            Node *current = node->right->left;
            if(hasArguments)
            {
                args = (char *)realloc(args, 0);
                while(current != NULL)
                {
                    unsigned char mutable = (current->modifiers & MODIFIER_MUTABLE) != 0;
                    char *argTypeString = nodeTypeToString(current->left);
                    char *argIdentifierString = strdup(current->right->value.string_value);
                    size_t argsStringSize = strlen(args) + strlen(argTypeString) + strlen(argIdentifierString) + 3 + (mutable ? 0 : 6); // (existing)_TYPE_IDENTIFIER
                    args = (char *)realloc(args, argsStringSize);

                    if(!mutable) strcat(args, "const ");
                    strcat(args, argTypeString);
                    strcat(args, " ");
                    strcat(args, argIdentifierString);

                    if(current->next != NULL) strcat(args, ", ");
                    free(argIdentifierString);

                    current = current->next;
                }
            }

            unsigned char hasBody = node->right->right != NULL;
            char *bodyString;
            if(hasBody)
            {
                bodyString = parseNode(node->right->right);
            }

            size_t resultSize = strlen(typeString) + strlen(identifierString) + 5 + (constant ? 6 : 0) + strlen(args) + (hasBody ? strlen(bodyString) : 0);
            char *result = (char *)malloc(resultSize);

            snprintf(result, resultSize, "%s%s %s(%s)%s", (constant ? "const ": ""), typeString, identifierString, args, (hasBody ? bodyString : ""));

            return result;
        }
        case NODE_FUNCTION_CALL:
        {
            char *identifierString = node->value.string_value;

            char *args = (char *)malloc(1);
            unsigned char hasArguments = node->left != NULL;
            Node *current = node->left;
            if(hasArguments)
            {
                while(current != NULL)
                {
                    char *value = parseNode(current);
                    size_t argsStringSize = strlen(args) + strlen(value) + 3;
                    args = (char *)realloc(args, argsStringSize);

                    strcat(args, value);
                    if(current->next != NULL) strcat(args, ", ");

                    current = current->next;
                }
            }

            size_t resultSize = strlen(identifierString) + 3 + strlen(args);
            char *result = (char *)malloc(resultSize);

            snprintf(result, resultSize, "%s(%s)", identifierString, args);

            free(args);
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
        case NODE_RETURN:
        {
            char *expressionString = parseNode(node->left);
            size_t length = 8 + strlen(expressionString);
            char *result = (char *)malloc(length);

            strcat(result, "return ");
            strcat(result, expressionString);

            free(expressionString);
            return result;
        }
        case NODE_COMPILER_MACRO:
        {
            char *macroName = node->value.string_value;

            char *args = (char *)malloc(1);
            unsigned char hasArguments = node->left != NULL;
            Node *current = node->left;
            if(hasArguments)
            {
                while(current != NULL)
                {
                    char *value = parseNode(current);
                    size_t argsStringSize = strlen(args) + strlen(value) + 2;
                    args = (char *)realloc(args, argsStringSize);

                    strcat(args, value);
                    if(current->next != NULL) strcat(args, " ");

                    current = current->next;
                }
            }

            size_t resultSize = strlen(macroName) + 4 + strlen(args);
            char *result = (char *)malloc(resultSize);

            snprintf(result, resultSize, "#%s %s\n", macroName, args);

            free(args);
            return result;
        }
        case NODE_VALUE_STRING:
        {
            size_t resultSize = strlen(node->value.string_value) + 3;
            char *result = (char *)malloc(resultSize);

            snprintf(result, resultSize, "\"%s\"", node->value.string_value);

            return result;
        }
        case NODE_SCOPE:
        {
            char *result = strdup("{ ");

            Node *current = node->left;
            while (current != NULL)
            {
                char *stmtString = parseNode(current);
                result = realloc(result, strlen(result) + strlen(stmtString) + 1);
                strcat(result, stmtString);
                free(stmtString);
                current = current->next;
            }

            result = realloc(result, strlen(result) + 3);
            strcat(result, "}; ");
            return result;
        }
        default:
        {
            printf("UNEXPECTED NODE: %d\n", node->type);
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