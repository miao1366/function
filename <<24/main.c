#include <time.h>
#include <stdio.h>
#include <stdint.h>


#define PTYPE_TEXT 0
#define MESSAGE_TYPE_SHIFT ((sizeof(size_t)-1) * 8)
#define HANDLE_REMOTE_SHIFT 24

int main() {
	int harbor = 1;
	harbor = (uint32_t)( 0xff) << HANDLE_REMOTE_SHIFT;
	printf("%u\n", harbor);
	void *p = (void *)(intptr_t)(~0);
	printf("%p\n", p);

   
	int len = 3;
	size_t sz = len | ((size_t)PTYPE_TEXT << MESSAGE_TYPE_SHIFT);
	printf("%zu\n", sz);
  
	int m = 4;
    uintptr_t v = (uint32_t)(-m);
	printf("%lu\n",v);
	
	m = 3;
    v = (uint32_t)(-m);
	printf("%lu\n",v);
	return 0;
}
