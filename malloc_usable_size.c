#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

int main() {
    void *p1 = malloc(sizeof(int));
    printf("malloc p1: %ld\n", malloc_usable_size(p1));
    
    char *p2 = malloc(1);
    printf("malloc p2: %ld\n", malloc_usable_size(p2 + 0));
    printf("malloc p2: %ld\n", malloc_usable_size(p2 + 5));
    printf("malloc p2: %ld\n", malloc_usable_size(p2 + 10));
    printf("malloc p2: %ld\n", malloc_usable_size(p2 + 15));
    printf("malloc p2: %ld\n", malloc_usable_size(p2 + 20));
    printf("malloc p2: %ld\n", malloc_usable_size(p2 + 25));
    printf("malloc p2: %ld\n", malloc_usable_size(p2 + 30));
  
    char *p3 = (char*)malloc(25);
    p3[0] = 'a';
    p3[1] = 'a';
    p3[2] = 'a';
    p3[3] = 'a';

    printf("malloc p3: %ld\n", malloc_usable_size(p3));
  
    void *p4 = malloc(1000);
    printf("malloc p4: %ld\n", malloc_usable_size(p4));
  
    size_t a = 0;
    return 0;
}