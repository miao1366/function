#include <stdio.h>

struct hh {
#define aaa 1
#define bbb 2
int  a;
};

#define	_TAILQ_ENTRY(type, qual)					\
struct {								\
	qual type *tqe_next;		/* next element */		\
	qual type *qual *tqe_prev;	/* address of previous next element */\
}
#define TAILQ_ENTRY(type)	_TAILQ_ENTRY(struct type,)

struct tai {
    int value;
    TAILQ_ENTRY(tai);
};

// struct hh {
// int a;
// };
// struct tai {
//     int value;
//     struct { struct tai *tqe_next; struct tai * *tqe_prev; };
// };

int main() {
#if HSW
    printf("hello shanghai\n");
#else
    printf("hello beijing\n");
#endif
    printf("struct hh::aaa: %d\n", aaa);
    return 0;
}