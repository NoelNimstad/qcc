/**
 * @brief Qlang v2
 * @author QuantVasa
*/

#include "qlang.h"

int main(int argc, char const *argv[])
{
	ASSERT(argc >= 3, "Not enough arguments supplied.\n\tProper usage: qlang <IN.Q> <OUT>\n");

	{ // Read file
		FILE *filePointer = fopen(argv[1], "r");
		ASSERT(filePointer != NULL, "Input file \"%s\" does not exist.\n", argv[1]);

		fseek(filePointer, 0L, SEEK_END);
		size_t fileSize = ftell(filePointer);
		rewind(filePointer);

		if(fileSize == 0)
		{
			printWarning("Input file is empty", NULL);
		}

		qlang.fileContents = (char *)malloc(fileSize);
		ASSERT(qlang.fileContents != NULL, "Failed to allocate memory for file contents.\n");

		fread(qlang.fileContents, 1, fileSize, filePointer);

		fclose(filePointer);
	}

	destroyQlangStruct(&qlang);
	return EXIT_SUCCESS;
};