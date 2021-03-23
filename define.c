#include <stdio.h>

struct hh {
#define aaa 1
#define bbb 2
int  a;
};


int main() {
#if HSW
    printf("hello shanghai\n");
#else
    printf("hello beijing\n");
#endif
    printf("struct hh::aaa: %d\n", aaa);
    return 0;
}