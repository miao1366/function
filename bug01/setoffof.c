#include <stdio.h>
#include <stddef.h>

//#define  offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

struct st {
    int a;
    int b;
};

struct list_head {
    struct list_head *next, *prev;
};


int main() {
    struct st s;
    struct st *p = NULL;
    int a = 4;
    printf("offset:%ld\n", offsetof(struct st, b));
    printf("offset:%ld\n", offsetof(struct st, a));
    printf("offset:%ld\n", (size_t)&(p->a));
    sizeof(++a);
    printf("offset:%ld\n", (size_t)&(p->a));
    printf("a->:%d\n", a);
}
