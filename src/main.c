#include <stdio.h>
#include <stdlib.h>

#include "tokens.h"
#include "tokenize.h"
#include "tree.h"
#include "debug.h"
#include "parse.h"

int main(int argc, char const *argv[])
{
	if(argc < 3)
	{
		printf("Invalid number of arguments.\n");
		return 1;
	}

	FILE *filePointer = fopen(argv[1], "r");
	if(filePointer == NULL)
	{
		printf("Unable to open input file.\n");
		fclose(filePointer);
		return 1;
	}

	fseek(filePointer, 0, SEEK_END);
	long fileSize = ftell(filePointer);
	fseek(filePointer, 0, SEEK_SET);

	char *fileContents = malloc(fileSize + 1);
	fread(fileContents, fileSize, 1, filePointer);
	fclose(filePointer);
	fileContents[fileSize] = 0;

	Token *tokens = tokenize(fileContents);
	Node *headNode = generateNodeTree(tokens);
	char *result = parseTree(headNode);

	filePointer = fopen(argv[2], "w");
	if(filePointer == NULL)
	{
		printf("Unable to open output file.\n");
		fclose(filePointer);
		free(result);
		freeNodeTree(headNode);
		freeTokens(tokens);
		free(fileContents);
		return 1;
	}

	fprintf(filePointer, "%s", result);
	fclose(filePointer);

	free(result);
	freeNodeTree(headNode);
	freeTokens(tokens);
	free(fileContents);
	return 0;
}