#ifndef QLANG_H
#define QLANG_H

// Includes
#include <stdio.h>
#include <stdlib.h>

// Qlang struct
struct QLANG_STRUCT
{
    char *fileContents;
    unsigned int line, column;
}; struct QLANG_STRUCT qlang;

// Colors
#define COLOR_RED    "\x1B[31m"
#define COLOR_YELLOW "\x1B[33m"
#define COLOR_GREEN  "\x1B[32m"
#define COLOR_RESET  "\x1B[0m"

// Debug
void printError(const char *reason, const char *help);   // For printing fatal errors
void printWarning(const char *reason, const char *help); // For printing non fatal warnings

// Errors
#define ASSERT(_e, ...) if(!(_e)){ fprintf(stderr, COLOR_RED "Qlang error" COLOR_RESET ": "); fprintf(stderr, __VA_ARGS__); exit(EXIT_FAILURE); }

// Cleanup
void destroyQlangStruct(struct QLANG_STRUCT *qlang);

#endif // QLANG_H