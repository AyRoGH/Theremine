#ifndef __WAIT_H__
#define __WAIT_H__

#include "mbed.h"

void wait_ms(int ms) {
    wait_us(ms * 1000);
}

void wait_s(int s) {
    wait_us(s * 1000000);
}

#endif // __WAIT_H__