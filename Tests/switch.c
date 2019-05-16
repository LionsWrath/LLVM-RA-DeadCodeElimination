#include <stdio.h>
#include <stdlib.h>

int f(int i, int j) {
    int x, y;
    switch(i) {
    case 0: x = 3; break;
    case 1: x = 1; break;
    case 2: x = 4; break;
    case 3: x = 1; break;
    case 4: x = 5; break;
    default: x = 9;
    }
    switch(j) {
    case 0: y = x + 2; break;
    case 1: y = x + 7; break;
    case 2: y = x + 1; break;
    case 3: y = x + 8; break;
    case 4: y = x + 2; break;
    default: y = x + 8;
    }
    return y;
}

int main()
{
    f(1, 2);
    return 0;
}
