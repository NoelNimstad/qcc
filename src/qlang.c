#include "qlang.h"

void debugPrintTokens(struct qlangStruct *qlang)
{
	printf(COLOR_GREEN "Qlang debug " COLOR_RESET "(tokens):\n");

	int i = 0;
	for(struct token *currentToken = qlang->tokens; currentToken->type != TOKEN_END_OF_FILE; currentToken++)
	{
		i++;
		switch(currentToken->type)
		{
			case TOKEN_TYPE_INT:
				printf("\tTYPE_INT\n");
				break;
			case TOKEN_IDENTIFIER:
				printf("\tIDENTIFIER(%s)\n", currentToken->value.string);
				break;
			case TOKEN_OPERATOR_ASSIGN:
				printf("\tOPERATOR_ASSIGN\n");
				break;
			case TOKEN_VALUE_INT:
				printf("\tINT(%d)\n", currentToken->value.integer);
				break;
			case TOKEN_SEMI_COLON:
				printf("\tSEMI_COLON\n");
				break;
			default:
				break;
		}
    }

	printf(COLOR_GREEN "\tEND_OF_FILE (%d tokens printed)\n" COLOR_RESET, i + 1);
}

void destroyQlangStruct(struct qlangStruct *qlang)
{
    free(qlang->fileContents);

	for(struct token *currentToken = qlang->tokens; currentToken->type != TOKEN_END_OF_FILE; currentToken++)
	{
        if(currentToken->type == TOKEN_IDENTIFIER && currentToken->value.string != NULL)
		{
            free(currentToken->value.string); // Free used strdup
        }
    }
	free(qlang->tokens);
}

void processFile(const char *path)
{
    struct qlangStruct qlang;

    { // Read file
		FILE *filePointer = fopen(path, "r");
		ASSERT(filePointer != NULL, "Input file \"%s\" does not exist.\n", path);

		fseek(filePointer, 0L, SEEK_END);
		size_t fileSize = ftell(filePointer);
		rewind(filePointer);

		SOFT_ASSERT(fileSize != 0, "Input file \"%s\" is empty.\n", path);

		qlang.fileContents = (char *)malloc(fileSize);
		ASSERT(qlang.fileContents != NULL, "Failed to allocate memory (%zu bytes) for file contents.\n", fileSize);

		fread(qlang.fileContents, 1, fileSize, filePointer);

		fclose(filePointer);
    }

	qlang.tokens = generateTokens(qlang.fileContents);
	debugPrintTokens(&qlang);

    destroyQlangStruct(&qlang);
}
