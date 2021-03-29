#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int m = 0;
    for (int i = 0; i < 3; i++) {
        m++;
    }
    char* p = (char*)realloc(100, 2);
    p[0] = 'a';
    p[1] = 0;
    printf("%s\n", p);
}