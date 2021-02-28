#ifndef TIMESTAMP_H__
#define TIMESTAMP_H__

#include <stdint.h>  //int64_t

class TimeStamp {

public:
    TimeStamp(int64_t microseconds):m_microseconds(microseconds) {
    
    }
    TimeStamp():m_microseconds(0) {
    
    }
private:
    int64_t   m_microseconds;

};

#endif  //TIMESTAMP_H__