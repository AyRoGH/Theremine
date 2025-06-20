#ifndef __BUZZER_H__
#define __BUZZER_H__

#include "mbed.h"
#include "capteur.h"
#include "note.h"

class Buzzer
{

private:

    PwmOut buzzer;
    float prev_write_value = 0.25f;

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
            if (prev_write_value == 0.1f)
            {
                buzzer.write(0.1f);
                prev_write_value = 0.1f;
            }
            break;
        case Note::G4:
        case Note::G5:
        default:
            if (prev_write_value == 0.25f)
            {
                buzzer.write(0.25f);
                prev_write_value = 0.25f;
            }
            break;
    }
}

void Buzzer::mute(void)
{
    buzzer.period(0.0f);
    buzzer.write(0.0f);
}

#endif // __BUZZER_H__