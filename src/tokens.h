#ifndef TOKENS_H
#define TOKENS_H

typedef enum
{
	// types
	TOKEN_TYPE_uint8,
	TOKEN_TYPE_sint8,
	TOKEN_TYPE_uint16,
	TOKEN_TYPE_sint16,
	TOKEN_TYPE_uint32,
	TOKEN_TYPE_sint32,
	TOKEN_TYPE_uint64,
	TOKEN_TYPE_sint64,
	TOKEN_TYPE_INT,
	TOKEN_TYPE_FLOAT,
	TOKEN_TYPE_double64,
	TOKEN_TYPE_double128,
	TOKEN_TYPE_DOUBLE,
	TOKEN_TYPE_STRING,
	TOKEN_TYPE_VOID,

	// values
	TOKEN_VALUE_INT,
	TOKEN_VALUE_FLOAT,
	TOKEN_VALUE_DOUBLE,

	// variables
	TOKEN_IDENTIFIER,

	// brackets
	TOKEN_LEFT_ROUND_BRACKET,
	TOKEN_RIGHT_ROUND_BRACKET,
	TOKEN_LEFT_CURLY_BRACKET,
	TOKEN_RIGHT_CURLY_BRACKET,

	// operators
	TOKEN_OPERATOR_SEMI_COLON,
	TOKEN_OPERATOR_PLUS,
	TOKEN_OPERATOR_MINUS,
	TOKEN_OPERATOR_MULTIPLY,
	TOKEN_OPERATOR_DIVIDE,
	TOKEN_OPERATOR_ASSIGN,

	// keywords
	TOKEN_RETURN,

	// miscelanious
	TOKEN_ERROR,
	TOKEN_NEW_LINE,
	TOKEN_END_OF_FILE,
	TOKEN_COMMA,
	TOKEN_MODIFIER
} TokenType;

typedef struct
{
    TokenType type;

	union
	{
		char *string_value;
		int int_value;
		float float_value;
		double double_value;
	} value;
} Token;

void freeTokens(Token *tokens)
{
    for (Token *t = tokens; t->type != TOKEN_END_OF_FILE; t++)
	{
        if (t->type == TOKEN_IDENTIFIER && t->value.string_value != NULL)
		{
            free(t->value.string_value);
        }
    }
    free(tokens);
}

#endif // TOKENS_H