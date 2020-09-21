#include <time.h>
#include <stdio.h>
#include <stdint.h>

#define HANDLE_REMOTE_SHIFT 24

int main() {
	int harbor = 1;
	harbor = (uint32_t)(harbor & 0xff) << HANDLE_REMOTE_SHIFT;
	printf("%u\n", harbor);
	return 0;
}
