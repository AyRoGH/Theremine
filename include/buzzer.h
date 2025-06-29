#ifndef __BUZZER_H__
#define __BUZZER_H__

#include "mbed.h"
#include "capteur.h"
#include "note.h"

class Buzzer
{

private:

    PwmOut buzzer;

public:

    Buzzer(PinName pin_buzzer);

    void play(Note note);
    void mute(void);

};

Buzzer::Buzzer(PinName pin_buzzer) : buzzer(pin_buzzer) {}

void Buzzer::play(Note note)
{
    buzzer.period(1.0f / note.getFreq());
    switch (note.gamme)
    {
        case Note::G3:
            buzzer.write(0.1f);
            break;
        case Note::G4:
        case Note::G5:
        default:
            buzzer.write(0.25f);
            break;
    }
}

void Buzzer::mute(void)
{
    buzzer.period(0.0f);
    buzzer.write(0.0f);
}

#endif // __BUZZER_H__