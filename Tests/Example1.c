#include <stdio.h>
#include <stdlib.h>

int foo1(int k) {
    int i = 0;
    if (k % 2) {
        i++;
    }
    return i & 1;
}

int main(int argc, char** argv) {
    foo1(4);

    return 0;
}
