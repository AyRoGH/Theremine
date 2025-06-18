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

    void setValue(char volume)
    {
        char value;

        if (volume >= 0 && volume < 6)
            value = 0;
        else if (volume >= 6 && volume < 10)
            value = 1;
        else if (volume >= 10 && volume < 14)
            value = 2;
        else if (volume >= 14 && volume < 18)
            value = 3;
        else if (volume >= 18 && volume < 22)
            value = 4;
        else if (volume >= 22 && volume < 26)
            value = 5;
        else if (volume >= 26 && volume <= 32)
            value = 6;

        switch (value)
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