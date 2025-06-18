#ifndef __LED_VOLUME_H__
#define __LED_VOLUME_H__

#include "mbed.h"

class LedVolume
{

private:

    DigitalOut d1;
    DigitalOut d2;
    DigitalOut d3;
    DigitalOut d4;
    DigitalOut d5;
    DigitalOut d6;

public:

    LedVolume(PinName pin_d1, PinName pin_d2, PinName pin_d3, PinName pin_d4, PinName pin_d5, PinName pin_d6)
        : d1(pin_d1), d2(pin_d2), d3(pin_d3), d4(pin_d4), d5(pin_d5), d6(pin_d6)
    {
        d1 = 0;
        d2 = 0;
        d3 = 0;
        d4 = 0;
        d5 = 0;
        d6 = 0;
    }

    void setValue(char volume_char) { // Renamed to avoid conflict with member
        unsigned char volume = static_cast<unsigned char>(volume_char); // Cast for safety if char is signed
        unsigned char value_to_set = 0; // Renamed to avoid conflict
        if (volume < 6) value_to_set = 0;
        else if (volume < 10) value_to_set = 1;
        else if (volume < 14) value_to_set = 2;
        else if (volume < 18) value_to_set = 3;
        else if (volume < 22) value_to_set = 4;
        else if (volume < 26) value_to_set = 5;
        else if (volume <= 32) value_to_set = 6; // Max volume 32 maps to 6 LEDs
        else value_to_set = 0; // Default for out of range (e.g. negative if char was signed and unhandled)

        // The switch statement remains the same but uses value_to_set
        switch (value_to_set)
        {
            case 0:
                d1 = 0;
                d2 = 0;
                d3 = 0;
                d4 = 0;
                d5 = 0;
                d6 = 0; 
                break;
            case 1:
                d1 = 1;
                d2 = 0;
                d3 = 0;
                d4 = 0;
                d5 = 0;
                d6 = 0; 
                break;
            case 2:
                d1 = 1;
                d2 = 1;
                d3 = 0;
                d4 = 0;
                d5 = 0;
                d6 = 0; 
                break;
            case 3:
                d1 = 1;
                d2 = 1;
                d3 = 1;
                d4 = 0;
                d5 = 0;
                d6 = 0; 
                break;
            case 4:
                d1 = 1;
                d2 = 1;
                d3 = 1;
                d4 = 1;
                d5 = 0;
                d6 = 0; 
                break;
            case 5:
                d1 = 1;
                d2 = 1;
                d3 = 1;
                d4 = 1;
                d5 = 1;
                d6 = 0; 
                break;
            case 6:
                d1 = 1;
                d2 = 1;
                d3 = 1;
                d4 = 1;
                d5 = 1;
                d6 = 1; 
                break;
        }
    }

};

#endif /* __LED_VOLUME_H__ */