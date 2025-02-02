#include "stdio.h"

int banana(int x, int y)
{
    return x + y;
}

int main()
{
    printf("Hello QCC: %d\n", banana(1, 5));
    return 0;
}