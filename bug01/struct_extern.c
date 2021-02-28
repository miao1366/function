#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct str {
    short type;
};

void * func(const char * str) {
    struct str * ptr_ =(struct str *)malloc(sizeof(*ptr_) + strlen(str) +1);
    if ( ! ptr_) {
        return NULL;
    }
    ptr_->type = 3;
    memcpy((void*)(ptr_ +1), str, strlen(str)+ 1);
    return ptr_;
}

int main() {
    struct str * ptr_ =(struct str *)func("hello");
    printf("str: %s\n", (char*)(ptr_ +1 ));
    return 0;
}