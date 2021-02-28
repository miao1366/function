#include <stdio.h>
#include <stdlib.h>

int main() {
    const char *s = getenv("MUDUO_LOG_TRACE");
    if (NULL == s) {
        return 0;
    }
    printf("%s\n", s);
    return 0;
}

//export MUDUO_LOG_TRACE=11