#include <unistd.h>
#include <sys/syscall.h>

__thread  int t_cachedTid = 0;

void cacheTid() {
    t_cachedTid = (int)syscall(SYS_gettid);
}

