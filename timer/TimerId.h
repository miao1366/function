#ifndef TIMERID_H__
#define TIMERID_H__

#include <stdint.h>

class Timer;

class TimerId {
public:
    TimerId():m_timer(nullptr), m_iSequence() {

    }

    TimerId(Timer *timer, int16_t sequence):m_timer(timer),m_iSequence(sequence) {
        
    }

private:
    Timer*    m_timer;
    int16_t   m_iSequence;
};

#endif  //TIMERID_H__