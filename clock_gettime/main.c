#include <time.h>
#include <stdio.h>


int main() {
	struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
	printf("second:: %ld, nsec:%ld", t.tv_sec, t.tv_nsec);

    clock_gettime(CLOCK_REALTIME, &t);
	printf("second:: %ld, nsec:%ld", t.tv_sec, t.tv_nsec);
}
