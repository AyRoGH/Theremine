#ifndef __RESISTANCE_H__
#define __RESISTANCE_H__

#include "mbed.h"
#include "wait.h"
#include <cstdint>

class Resistance
{
private:

    DigitalOut UD;
    DigitalOut INC;
    DigitalOut CS;

    uint8_t wiper_pos = 0; // [0..31]

public:

    Resistance(PinName pin_UD, PinName pin_INC, PinName pin_CS);
    void setValue(uint8_t new_wiper_pos);
};

Resistance::Resistance(PinName pin_UD, PinName pin_INC, PinName pin_CS)
    : UD(pin_UD), INC(pin_INC), CS(pin_CS)
{
    CS = 1;
    INC = 1;
    UD = 0;
}

void Resistance::setValue(uint8_t new_wiper_pos)
{
    if (new_wiper_pos > 31 || new_wiper_pos == wiper_pos) {
        return;
    }

    CS = 0;
    wait_ns(200);

    if (wiper_pos < new_wiper_pos) {
        UD = 1;
        wait_ns(100); // tDI >= 50 ns
        for (; wiper_pos < new_wiper_pos; wiper_pos++) {
            INC = 1;
            wait_us(10); // >> 250 ns (tIH)
            INC = 0;
            wait_us(10); // >> 250 ns (tIL)
            INC = 1;
            wait_us(10);
        }
    }
    else {
        UD = 0;
        wait_ns(100); // tDI >= 50 ns
        for (; wiper_pos > new_wiper_pos; wiper_pos--) {
            INC = 1;
            wait_us(10); // >> 250 ns
            INC = 0;
            wait_us(10); // >> 250 ns
            INC = 1;
            wait_us(10);
        }
    }

    INC = 1;
    wait_ns(100);

    CS = 1;
    wait_ms(6);   // > tWR (~5 ms)

}

#endif // __RESISTANCE_H__