#include <stdarg.h>
#include <stdio.h>

//  va_list是一个指向参数首地址的指针，
//  typedef struct {
//  char *a0; /* pointer to first homed integer argument */
//  int offset; /* byte offset of next parameter */
//  } va_list;


// void va_copy（va_list dest，va_list src）;
// va_copy宏将src复制到dest。
// va_end应在函数返回之前在dest上调用，或者任何后续的dest重新初始化（通过调用va_start或va_copy）


void foo(const char *fmt, ...) {
    va_list ap;
    int d;
    char c, *s;
    va_start(ap, fmt);
    while (*fmt)            //有点看不太懂啊
        switch (*fmt++) {
        case 's':              /* string */
            s = va_arg(ap, char *);
            printf("string %s\n", s);
            break;
        case 'd':              /* int */
            d = va_arg(ap, int);
            printf("int %d\n", d);
            break;
        case 'c':              /* char */
            /* need a cast here since va_arg only
            /* need a cast here since va_arg only
             takes fully promoted types */
            c = (char) va_arg(ap, int);
            printf("char %c\n", c);
            break;
        }
    va_end(ap);
}

int main(void) {
    foo("%s   %d %c %d %c", "Hello", 4, 'x', 3, 'y');
    return 0;
}