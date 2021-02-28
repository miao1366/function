#ifndef TIMERQUEUE_H__
#define TIMERQUEUE_H__

#include <vector>
#include <set>

#include "Timer.h"
#include "TimerId.h"
#include "Channel.h"
#include "TimeStamp.h"

class EventLoop;

class TimerQueue {
public:
    TimerQueue(EventLoop *eventLoop);

    TimerId addTimer(TimerCb timerCb, TimeStamp happenTime, int32_t intervalMsec);
private:
    bool    insert(Timer *timer);

private:
    typedef std::pair<TimeStamp, Timer*>  Entry;
    typedef std::set <Entry>  TimerList;
    typedef std::pair<Timer*, int64_t>    ActiveTimer;
    typedef std::set <ActiveTimer>        ActiveTimerSet; 
    
    EventLoop       *m_eventLoop;
    const int        m_iTimerFd;
    Channel          m_timerChannel;
    TimerList        m_timerList;
    ActiveTimerSet   m_activeTimeSet;
};
#endif  //TIMERQUEUE_H__