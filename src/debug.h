#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#include "tokens.h"

void _debugTokens(Token *tokens)
{
	printf("TOKENS:\n");
	for(Token *t = tokens; t->type != TOKEN_END_OF_FILE; t++) 
	{
		switch(t->type)
		{
			case TOKEN_LEFT_ROUND_BRACKET:
				printf("(\n");
				break;
			case TOKEN_RIGHT_ROUND_BRACKET:
				printf(")\n");
				break;
			case TOKEN_LEFT_CURLY_BRACKET:
				printf("{\n");
				break;
			case TOKEN_RIGHT_CURLY_BRACKET:
				printf("}\n");
				break;
			case TOKEN_OPERATOR_SEMI_COLON:
				printf(";\n");
				break;
			case TOKEN_IDENTIFIER:
				printf("IDENTIFIER, value=%s\n", t->value.string_value);
				break;
			case TOKEN_TYPE_INT:
				printf("TYPE_INT\n");
				break;
			case TOKEN_RETURN:
				printf("RETURN\n");
				break;
			case TOKEN_VALUE_INT:
				printf("INTEGER, value=%d\n", t->value.int_value);
				break;
			case TOKEN_VALUE_FLOAT:
				printf("FLOAT, value=%f\n", t->value.float_value);
				break;
			case TOKEN_VALUE_DOUBLE:
				printf("DOUBLE, value=%f\n", t->value.double_value);
				break;
			case TOKEN_TYPE_VOID:
				printf("VOID\n");
				break;
			case TOKEN_OPERATOR_MINUS:
				printf("MINUS\n");
				break;
			default:
				break;
		}
    }
	printf("\n");
}

#endif // DEBUG_H