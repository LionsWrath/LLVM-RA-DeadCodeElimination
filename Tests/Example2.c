#include <stdio.h>
#include <stdlib.h>

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
    foo2();

    return 0;
}
