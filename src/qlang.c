#include "qlang.h"

void destroyQlangStruct(struct QLANG_STRUCT *qlang)
{
    free(qlang->fileContents);
}