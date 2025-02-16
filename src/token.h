#ifndef TOKEN_H
#define TOKEN_H

#include "string.h"

enum tokenType
{
    TOKEN_TYPE_INT,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR_ASSIGN,  // =
    TOKEN_SEMI_COLON,       // ;
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