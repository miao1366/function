#include "TimerQueue.h"

#include <sys/timerfd.h>  //timerfd_create()

int createTimerfd() {
    int iTimerfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    if (iTimerfd < 0) {

    }
    return iTimerfd;
}

TimerQueue::TimerQueue(EventLoop *eventLoop):
        m_eventLoop(eventLoop),m_iTimerFd(createTimerfd()),
        m_timerChannel(eventLoop, m_iTimerFd),
        m_timerList() {

}


TimerId TimerQueue::addTimer(TimerCb timerCb, TimeStamp happenTime, int32_t intervalMsec) {
    
}


bool TimerQueue::insert(Timer * timer) {

}
