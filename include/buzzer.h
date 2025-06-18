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

Buzzer::Buzzer(PinName pin_buzzer) : buzzer(pin_buzzer), is_muted(false) {}

void Buzzer::handledMute(void)
{
    if (is_muted) {
        buzzer.write(0.0f); // Maintain mute
    } else {
        buzzer.write(0.25f); // Set to a default play volume (e.g., for gamme 4/5)
    }
}

void Buzzer::setMuteState(bool mute_state)
{
    is_muted = mute_state;
    handledMute(); // Apply the state immediately
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