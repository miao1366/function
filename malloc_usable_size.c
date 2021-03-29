#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

int main() {
    void *p1 = malloc(sizeof(int));
    printf("malloc p1: %d\n", malloc_usable_size(p1));
    
    void *p2 = malloc(0);
    printf("malloc p2: %d\n", malloc_usable_size(p2));
  
    void *p3 = malloc(25);
    printf("malloc p3: %d\n", malloc_usable_size(p3));
  
    void *p4 = malloc(1000);
    printf("malloc p4: %d\n", malloc_usable_size(p4));
  
    return 0;
}