#ifndef TOKEN_H
#define TOKEN_H

#include "string.h"

enum tokenType
{
    TOKEN_TYPE_INT,         // Type integer (int)
    TOKEN_VALUE_INT,        // Integer value, e.g -2, 0, 2
    TOKEN_IDENTIFIER,       // Identifiers / Names
    TOKEN_OPERATOR_ASSIGN,  // =
    TOKEN_SEMI_COLON,       // ;
    TOKEN_END_OF_FILE,      // \0
    TOKEN_ERROR,            // Unmatched tokens
};

struct token
{
    enum tokenType type;
    union
    {
        int integer;
        double decimal;
        char *string;
    } value;
};

#endif // TOKEN_H