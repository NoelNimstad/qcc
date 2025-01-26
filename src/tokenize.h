#ifndef TOKENIZE_H
#define TOKENIZE_H

#define DEFAULT_TOKENS 256
#define VARIABLE_NAME_MAX_LENGTH 256
#define NUMBER_MAX_LENGTH 256

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "tokens.h"
#include "keywords.h"
#include "modifiers.h"

Token *tokenize(char *file)
{
	Token *tokens = (Token *)malloc(sizeof(Token) * DEFAULT_TOKENS); 
	Token *current = &tokens[0];
	char *character = &file[0];

	while(*character != '\0')
	{
		// if((*character) == '\n')
		// {
		// 	current->type = TOKEN_NEW_LINE;
		// 	character++;
		// 	current++;
		// 	continue;
		// }

		if(isspace(*character))
		{
			character++;
			continue;
		}

		if(isalpha(*character))
		{
			char buffer[VARIABLE_NAME_MAX_LENGTH];
			int i = 0;

			while(isalnum(*character))
			{
				buffer[i++] = *character;
				character++;
			}
			buffer[i] = '\0';

			current->type = TOKEN_IDENTIFIER;
			for(Keyword *entry = keywordTable; entry->keyword != NULL; ++entry)
			{
				if(strcmp(buffer, entry->keyword) == 0)
				{
					current->type = entry->type;
				}
			}
			current->value.string_value = strdup(buffer);
			current++;
		}

		if(isdigit(*character))
		{
			char buffer[NUMBER_MAX_LENGTH];
			TokenType numberType = TOKEN_VALUE_INT;
			int i = 0;
			int hasDecimalPoint = 0;

			while((isdigit(*character) || *character == '.') && i < NUMBER_MAX_LENGTH - 1)
			{
				if(*character == '.')
				{
					if(hasDecimalPoint)
					{
						break;
					}
					hasDecimalPoint = 1;
					numberType = TOKEN_VALUE_DOUBLE;
				}
				buffer[i++] = *character;
				character++;
			}
			if(*character == 'f')
			{
				numberType = TOKEN_VALUE_FLOAT;
			}
			buffer[i] = '\0';

			switch (numberType)
			{
				case TOKEN_VALUE_INT:
					current->value.int_value = atoi(buffer);
					break;
				case TOKEN_VALUE_FLOAT:
					current->value.float_value = atof(buffer);
					break;
				case TOKEN_VALUE_DOUBLE:
					current->value.double_value = atof(buffer);
					break;
				default:
					break;
			}

			current->type = numberType;
			current++;
		}

		switch(*character)
		{
            case '(':
                current->type = TOKEN_LEFT_ROUND_BRACKET;
                current++;
                break;
			case '/':
				if(*(character+1) == '/')
				{
					character += 2;
					while(*character != '\n' && *character != '\0') // messed up spent so much time only to realize the problem was that the comment would eat up way too much if placed at the end of a file
					{
						character++;
					}
				} else
				{
					current->type = TOKEN_OPERATOR_DIVIDE;
					current++;
				}
				break;
            case ')':
                current->type = TOKEN_RIGHT_ROUND_BRACKET;
                current++;
                break;
            case '{':
                current->type = TOKEN_LEFT_CURLY_BRACKET;
                current++;
                break;
            case '}':
                current->type = TOKEN_RIGHT_CURLY_BRACKET;
                current++;
                break;
            case ';':
                current->type = TOKEN_OPERATOR_SEMI_COLON;
                current++;
                break;
            case '+':
                current->type = TOKEN_OPERATOR_PLUS;
                current++;
                break;
            case '-':
                current->type = TOKEN_OPERATOR_MINUS;
                current++;
                break;
            case '=':
                current->type = TOKEN_OPERATOR_ASSIGN;
                current++;
                break;
			case '@':
				current->type = TOKEN_MODIFIER;
				character++;

				char buffer[32];
				int i = 0;
				while(isalnum(*character))
				{
					buffer[i++] = *(character+1);
					character++;
				}
				buffer[i] = '\0';
				current->value.string_value = strdup(buffer);

				current++;
            default:
                break;
        }

		character++;
	}

	current->type = TOKEN_END_OF_FILE;

	return tokens;
}

#endif // TOKENIZE_H