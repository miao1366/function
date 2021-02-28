#ifndef ATOMICINTEGER_H__
#define ATOMICINTEGER_H__

#include <stdint.h>  //int64_t, int32_t

template<typename T>
class AtomicInteger {
public:
    AtomicInteger():m_iValue() {

    }
    T get() {
        return  __sync_val_compare_and_swap(&m_iValue, 0, 0);
    }
    T getAndAdd(T iAdd) {
        return  __sync_fetch_and_add(&m_iValue, iAdd);
    }
    T incrementAndGet() {
        return  getAndAdd(1);
    }
private:
    volatile T    m_iValue;
};

typedef AtomicInteger<int64_t>  AtomicInt64;
typedef AtomicInteger<int32_t>  AtomicInt32;
typedef AtomicInteger<int8_t>   AtomicInt8;

#endif  //ATOMICINTEGER_H__