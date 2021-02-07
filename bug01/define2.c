#include <stdio.h>

#define CFG_MALLOC_LEN  (CFG_ENV_SIZE + 1024*1024)
#define CFG_ENV_SIZE    0x4000

#ifdef MAX_TEST 
    #define MAX_SIZE MAX_TEST+1 
#else 
    #define MAX_SIZE 1 
#endif 

#define MAX_TEST 2 

int main(void) {
    printf("MAX_SIZE: %d\n", MAX_SIZE);
    return 0;
}