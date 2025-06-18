#ifndef __RGB_LED_H__
#define __RGB_LED_H__

#include <mbed.h>

class RGBLed
{

private:

    DigitalOut RLed;
    DigitalOut GLed;
    DigitalOut BLed;

public:

    enum color
    {
        RED, GREEN, BLUE,
        MAGENTA, YELLOW, CYAN,
        WHITE
    };

    RGBLed(PinName pin_red, PinName pin_green, PinName pin_blue);
    void setColor(RGBLed::color color);

};

RGBLed::RGBLed(PinName pin_red, PinName pin_green, PinName pin_blue)
    : RLed(pin_red), GLed(pin_blue), BLed(pin_blue)
{
    RLed = 0;
    GLed = 0;
    BLed = 0;
}

void RGBLed::setColor(RGBLed::color color)
{
    switch (color)
    {
        case RED:
            RLed = 1;
            GLed = 0;
            BLed = 0;
            break;
        case GREEN:
            RLed = 0;
            GLed = 1;
            BLed = 0;
            break;
        case BLUE:
            RLed = 0;
            GLed = 0;
            BLed = 1;
            break;
        case MAGENTA:
            RLed = 1;
            GLed = 0;
            BLed = 1;
            break;
        case YELLOW:
            RLed = 1;
            GLed = 1;
            BLed = 0;
            break;
        case CYAN:
            RLed = 0;
            GLed = 1;
            BLed = 1;
            break;
        case WHITE:
            RLed = 1;
            GLed = 1;
            BLed = 1;
            break;
    }

    return;
}


#endif /* __RGB_LED_H__ */