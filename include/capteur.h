#ifndef __CAPTEUR_H__
#define __CAPTEUR_H__

#include "mbed.h"

class Capteur
{

private:

    DigitalOut trig;
    DigitalIn echo;
    Timer timer;

public:

    Capteur(PinName pin_trig, PinName pin_echo) : trig(pin_trig), echo(pin_echo) {}

    long long getDistance(void);

};

long long Capteur::getDistance(void)
{
    trig = 0;
    wait_us(2);
    trig = 1;
    wait_us(10);
    trig = 0;

    while (!echo);
    timer.reset();
    timer.start();
    while (echo);
    timer.stop();

    return timer.elapsed_time().count() / 58;
}

#endif // __CAPTEUR_H__