#ifndef MODIFIERS_H
#define MODIFIERS_H

typedef enum
{
    MODIFIER_NONE    = 0,
    MODIFIER_MUTABLE = 1 << 0, // makes variable non-constant
    MODIFIER_CONST   = 1 << 1, // enforces const (on things like functions)
    MODIFIER_ONCE    = 1 << 2, // ensures some scope is only run once
} Modifier;

struct 
{
    const char *name;
    Modifier value;
} modifierTable[] = 
{
    { "mut", MODIFIER_MUTABLE },
    { "mutable", MODIFIER_MUTABLE },
    { "const", MODIFIER_CONST },
    { "constant", MODIFIER_CONST },
    { "once", MODIFIER_ONCE }
};

#endif