#include "msgqueue.h"
#include <stdio.h>

int main() {
    msgqueue_t * queue = msgqueue_create(100, sizeof(int));
    int *a  = new int(2);
    int *a1 = new int(22);
    int *a2 = new int(3);
    printf("a:%d, a1:%d, a2:%d\n", *a, *a1, *a2);
    printf("a:%d, a1:%d, a2:%d\n", a, a1, a2);
    msgqueue_put(a, queue);
    msgqueue_put(a1, queue);
    msgqueue_put(a2, queue);
    printf("put  put  put\n");
    printf("a:%d, a1:%d, a2:%d\n", a, a1, a2);
    printf("a:%d, a1:%d, a2:%d\n", *a, *a1, *a2);

    printf("get  get  get\n");
    int *p1 = (int*)msgqueue_get(queue);
    int *p2 = (int*)msgqueue_get(queue);
    int *p3 = (int*)msgqueue_get(queue);
    printf("p1:%d, p2:%d, p3:%d\n", p1, p2, p3);
    printf("p1:%d, p2:%d, p3:%d\n", *p1, *p2, *p3);
    msgqueue_get(queue);
    return 0;
}