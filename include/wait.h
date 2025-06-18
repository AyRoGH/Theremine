#ifndef __WAIT_H__
#define __WAIT_H__

#include "mbed.h"

// For new development or consistency with Mbed OS RTOS features,
// consider using mbed::ThisThread::sleep_for(chrono::milliseconds(ms))
// and mbed::ThisThread::sleep_for(chrono::seconds(s)).
void wait_ms(int ms) {
    wait_us(ms * 1000);
}

void wait_s(int s) {
    wait_us(s * 1000000);
}

#endif // __WAIT_H__