#ifndef TOKEN_H
#define TOKEN_H

enum tokenType
{
    TOKEN_TYPE_INT,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR_ASSIGN,  // =
    TOKEN_SEMI_COLON,       // ;
};

struct token
{
    tokenType type;
    union
    {
        int integer;
        double decimal;
        char *string;
    } value;
};

#endif // TOKEN_H