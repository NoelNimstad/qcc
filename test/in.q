@mut int x = 4;   // make x mutable
int y = 6; // y defaults to a constant
x = y;            // set x to y

int add(int a, @mut int b);