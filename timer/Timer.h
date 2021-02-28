#ifndef TIMER_H__
#define TIMER_H__

#include "TimeStamp.h"
#include "AtomicInteger.h"

typedef void (*TimerCb)(void * data); 

class Timer {
public:
    Timer(TimerCb timerCb, TimeStamp happenTime, int32_t intervalMsec):
        m_timerCb(timerCb),
        m_happenTime(happenTime), 
        m_iIntervalMsce(intervalMsec),
        m_iSequence(s_timerSum.incrementAndGet()),
        m_bRepeat(intervalMsec > 0) {
        
    }

private:
    TimerCb      m_timerCb;
    TimeStamp    m_happenTime;
    int32_t      m_iIntervalMsce;
    bool         m_bRepeat;
    int8_t       m_iSequence;
    static       AtomicInt8    s_timerSum;
};


#endif  //TIMER_H__