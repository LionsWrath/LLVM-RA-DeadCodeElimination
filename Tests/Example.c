#include <stdio.h>
#include <stdlib.h>

int foo1(int k) {
    int i = 0;
    if (k % 2) {
        i++;
    }
    return i & 1;
}

void foo2() {
    int i = 0;
    int sum = 0;
    for (i = 0; i < 100; i++) {
        sum += i;
        if (i > 101) {
            break;
        }
    }
}

int main(int argc, char** argv) {
    foo1(4);
    foo2();

    return 0;
}
