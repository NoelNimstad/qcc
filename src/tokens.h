#ifndef TOKENS_H
#define TOKENS_H

typedef enum
{
	// types
	TOKEN_TYPE_uint8,     // 1 byte unsigned integer
	TOKEN_TYPE_sint8,     // 1 byte signed integer
	TOKEN_TYPE_uint16,    // 2 byte unsigned integer
	TOKEN_TYPE_sint16,    // 2 byte signed integer
	TOKEN_TYPE_uint32,    // 4 byte unsigned integer
	TOKEN_TYPE_sint32,    // 4 byte signed integer
	TOKEN_TYPE_uint64,    // 8 byte unsigned integer
	TOKEN_TYPE_sint64,    // 8 byte signed integer
	TOKEN_TYPE_INT,       // integer (4 or 8 bytes)
	TOKEN_TYPE_FLOAT,     // floating point number
	TOKEN_TYPE_double64,  // 8 byte double
	TOKEN_TYPE_double128, // 16 byte double
	TOKEN_TYPE_DOUBLE,    // double (8 byte double)
	TOKEN_TYPE_STRING,    // TBD
	TOKEN_TYPE_VOID,      // void

	// values
	TOKEN_VALUE_INT,    // integer value
	TOKEN_VALUE_FLOAT,  // floating point value
	TOKEN_VALUE_DOUBLE, // double value

	// variables
	TOKEN_IDENTIFIER, // identifier name

	// brackets
	TOKEN_LEFT_ROUND_BRACKET,  // (
	TOKEN_RIGHT_ROUND_BRACKET, // )
	TOKEN_LEFT_CURLY_BRACKET,  // {
	TOKEN_RIGHT_CURLY_BRACKET, // }

	// operators
	TOKEN_OPERATOR_SEMI_COLON, // ;
	TOKEN_OPERATOR_PLUS,       // +
	TOKEN_OPERATOR_MINUS,      // -
	TOKEN_OPERATOR_STAR,   	   // *
	TOKEN_OPERATOR_DIVIDE,     // /
	TOKEN_OPERATOR_ASSIGN,     // =

	// keywords
	TOKEN_RETURN, // return keyword

	// miscelanious
	TOKEN_ERROR,       // error token, default
	TOKEN_NEW_LINE,    // \n
	TOKEN_END_OF_FILE, // for the end of a file
	TOKEN_COMMA,       // ,
	TOKEN_MODIFIER     // modifier token, like mutable
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