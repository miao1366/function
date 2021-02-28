#ifndef EVENTLOOP_H__
#define EVENTLOOP_H__

extern __thread  int t_cachedTid;


void cacheTid();

static inline int tid() {
    if (__builtin_expect(t_cachedTid == 0, 0)) {
        cacheTid();
    }
    return t_cachedTid;
}

#endif  //EVENTLOOP_H__