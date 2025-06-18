#ifndef __BUZZER_H__
#define __BUZZER_H__

#include "mbed.h"
#include "capteur.h"
#include "note.h"

class Buzzer
{

private:

    PwmOut buzzer;
    bool is_muted;

    void handledMute(void);

public:

    Buzzer(PinName pin_buzzer);

    void play(Note note);
    void mute(void);

};

Buzzer::Buzzer(PinName pin_buzzer) : buzzer(pin_buzzer) {}

void Buzzer::handledMute(void)
{
    if (is_muted)
        buzzer.write(0.0f);
    else
    is_muted = false;
        buzzer.write(0.25f); // Gamme 3 = 0.1, gamme 4 et 5 = 0.25
}

void Buzzer::play(Note note)
{
    buzzer.period(1.0f / note.getFreq());
    buzzer.write(0.25f);
}

void Buzzer::mute(void)
{
    buzzer.period(0.0f);
    buzzer.write(0.0f);
}

#endif // __BUZZER_H__