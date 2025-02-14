#include "qlang.h"

void printError(const char *reason, const char *help)
{
    if(help != NULL)
    {
        fprintf(stderr, COLOR_RED "Qlog error" COLOR_RESET ": %s\n\t%s\n", reason, help);
    } else 
    {
        fprintf(stderr, COLOR_RED "Qlang error" COLOR_RESET ": %s\n", reason);
    }
}

void printWarning(const char *reason, const char *help)
{
    if(help != NULL)
    {
        fprintf(stderr, COLOR_YELLOW "Qlang warning" COLOR_RESET ": %s\n\t%s\n", reason, help);
    } else 
    {
        fprintf(stderr, COLOR_YELLOW "Qlang warning" COLOR_RESET ": %s\n", reason);
    }
}

void destroyQlangStruct(struct QLANG_STRUCT *qlang)
{
    free(qlang->fileContents);
}