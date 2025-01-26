#ifndef KEYWORDS_H
#define KEYWORDS_H

#include <stdio.h>

#include "tokens.h"

typedef struct
{
    const char *keyword;
    TokenType type;
} Keyword;

Keyword keywordTable[] =
{
    { "int", TOKEN_TYPE_INT },
    { "uint8", TOKEN_TYPE_uint8 },
    { "sint8", TOKEN_TYPE_sint8 },
    { "uint16", TOKEN_TYPE_uint16 },
    { "sint16", TOKEN_TYPE_sint16 },
    { "uint32", TOKEN_TYPE_uint32 },
    { "sint32", TOKEN_TYPE_sint32 },
    { "uint64", TOKEN_TYPE_uint64 },
    { "sint64", TOKEN_TYPE_sint64 },
    { "float", TOKEN_TYPE_FLOAT },
    { "double", TOKEN_TYPE_DOUBLE },
    { "double64", TOKEN_TYPE_double64 },
    { "double128", TOKEN_TYPE_double128 },
    { "void", TOKEN_TYPE_VOID },
    { "return", TOKEN_RETURN },
};

#endif // KEYWORDS_H