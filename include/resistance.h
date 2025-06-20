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
    void reset();
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
    if (new_wiper_pos > 31) {
        return;
    }

    uint8_t target_pos = new_wiper_pos;

    CS = 0;
    wait_us(1); // Ensure CS setup time

    if (wiper_pos < target_pos) {
        UD = 1;
        wait_us(1); // Ensure direction setup time
        for (; wiper_pos < target_pos; wiper_pos++) {
            INC = 0;
            wait_us(1);
            INC = 1;
            wait_us(1);
        }
    }
    else if (wiper_pos > target_pos) {
        UD = 0;
        wait_us(1); // Ensure direction setup time
        for (; wiper_pos > target_pos; wiper_pos--) {
            INC = 0;
            wait_us(1);
            INC = 1;
            wait_us(1);
        }
    }

    CS = 1;
    wait_ms(10);   // Ensure write time

    // Force position update for reliability
    wiper_pos = target_pos;
}

void Resistance::reset()
{
    // Force reset to position 0
    wiper_pos = 31; // Set to max so setValue(0) will definitely work
    setValue(0);    // Reset to minimum position
}

#endif // __RESISTANCE_H__