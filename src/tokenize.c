#include "tokenize.h"

struct token *generateTokens(char *source)
{
    SOFT_ASSERT(strlen(source) != 0, "Input string to tokenizer is empty.\n");

    struct token* tokens = (struct token *)malloc(0xff * sizeof(struct token)); // Default to 255 tokens
    char *currentCharacter = source;

    while(*currentCharacter != '\0')
    {
        if(isspace(*currentCharacter)) // Ignore spaces
        {
            currentCharacter++;
            continue;
        }

        printf("%c", *currentCharacter);
        currentCharacter++;
    }

    return tokens;
}