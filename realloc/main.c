#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main() {
    char* p = (char*)realloc(NULL, 2);
    p[0] = 'a';
    p[1] = 0;
    printf("%s\n", p);
}