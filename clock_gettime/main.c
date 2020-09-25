#include <time.h>
#include <stdio.h>
#include <sys/time.h>

int main() {
	struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
	printf("second:: %ld, nsec:%ld\n", t.tv_sec, t.tv_nsec);

    clock_gettime(CLOCK_REALTIME, &t);
	printf("second:: %ld, nsec:%ld\n", t.tv_sec, t.tv_nsec);

	struct timeval tv;
	gettimeofday(&tv, NULL);

	printf("second:: %ld, nsec:%lu\n", tv.tv_sec, tv.tv_usec);
}
