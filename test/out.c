#include "stdio.h"
int banana(const int x, const int y) { return x + y; };
int main(void)
{
    printf("Hello QCC: %d\n", banana(1, 5));
    return 0;
};