#include "qlang.h"

void destroyQlangStruct(struct qlangStruct *qlang)
{
    free(qlang->fileContents);
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

    destroyQlangStruct(&qlang);
}
