#ifndef __TEREMINE_H__
#define __TEREMINE_H__

#include "mbed.h"
#include "capteur.h"
#include "note.h"
#include "buzzer.h"
#include "resistance.h"
#include "led_volume.h"
#include "rgb_led.h"
#include "wait.h"

#define MESURE_MOYENNE 0
#define DEMI_TON 1

class Theremine
{

private:

    Capteur gammeCapt;
    Capteur noteCapt;
    Capteur volumeCapt;
    Resistance resistance;
    Buzzer buzzer;
    LedVolume ledVolume;
    RGBLed ledGamme;

    static const int capt_min_dist = 5;
    #if DEMI_TON == 0
    static const int nb_note_per_gamme = 7;
    #else
    static const int nb_note_per_gamme = 12;
    #endif
    const int nb_gamme = 3;
    const int nb_volume = 32;

    #if DEMI_TON == 0
    const int dist_btw_note = 4;
    #else
    const int dist_btw_note = 2;
    #endif
    const int dist_btw_gamme = 10;

    static const Note default_note;

    Note prev_note = default_note;

    static const Note::Name note_name_selector[nb_note_per_gamme];
    static const Note::Gamme note_gamme_selector[3];
    static const RGBLed::color rgb_led_color_selector[3];

    unsigned char last_volume = 25U;

public:

    Theremine(Capteur gamme, Capteur note, Capteur volume, Resistance resistance, Buzzer buzzer, LedVolume ledVolume, RGBLed ledGamme);

    void cycle(void);
    void play_note(Note note, unsigned char volume);

};

const Note Theremine::default_note(Note::LA, Note::G4);

#if DEMI_TON == 0
const Note::Name Theremine::note_name_selector[] = {Note::DO, Note::RE, Note::MI, Note::FA, Note::SOL, Note::LA, Note::SI};
#else
const Note::Name Theremine::note_name_selector[] = {Note::DO, Note::DO_DIEZE, Note::RE, Note::RE_DIEZE, Note::MI, Note::FA, Note::FA_DIEZE, Note::SOL, Note::SOL_DIEZE, Note::LA, Note::LA_DIEZE, Note::SI};
#endif
const Note::Gamme Theremine::note_gamme_selector[3] = {Note::G3, Note::G4, Note::G5};
const RGBLed::color Theremine::rgb_led_color_selector[3] = {RGBLed::RED, RGBLed::GREEN, RGBLed::BLUE};

Theremine::Theremine(Capteur gamme, Capteur note, Capteur volume, Resistance resistance, Buzzer buzzer, LedVolume ledVolume, RGBLed ledGamme)
    : gammeCapt(gamme), noteCapt(note), volumeCapt(volume), resistance(resistance), buzzer(buzzer), ledVolume(ledVolume), ledGamme(ledGamme)
{
    this->resistance.reset();
    wait_ms(100); // Let the system stabilize
}
void Theremine::cycle(void)
{
    long long note_dist = noteCapt.getDistance();
    long long gamme_dist = gammeCapt.getDistance();
    long long volume_dist = volumeCapt.getDistance();
    int note_idx;
    int gamme_idx;
    unsigned char volume;
    Note::Name note_played;
    Note::Gamme gamme_played;
    char name[6] = {0};

    if (note_dist < capt_min_dist || note_dist > capt_min_dist + dist_btw_note * nb_note_per_gamme)
        note_dist = 0;
    if (gamme_dist < capt_min_dist || gamme_dist > capt_min_dist + dist_btw_gamme * nb_gamme)
        gamme_dist = 0;

    if (note_dist && gamme_dist)
    {
        note_idx = (note_dist - capt_min_dist) / dist_btw_note;
        gamme_idx = (gamme_dist - capt_min_dist) / dist_btw_gamme;
        
        if (volume_dist < capt_min_dist)
            volume = 0;
        else if (volume_dist > capt_min_dist + nb_volume - 1)
            volume = nb_volume - 1;
        else
            volume = volume_dist - capt_min_dist;

        note_played = note_name_selector[note_idx];
        gamme_played = note_gamme_selector[gamme_idx];

        Note curr_note(note_played, gamme_played);

        play_note(curr_note, volume);
        curr_note.getName(name);

        printf(">> %6s @ %02d [ n: %02lld cm | g: %02lld cm | v: %04lld cm ]\n", name, volume, note_dist, gamme_dist, volume_dist);
    }
    else
        buzzer.mute();

    wait_ms(100);

    return;
}

void Theremine::play_note(Note note, unsigned char volume)
{
    resistance.setValue(volume);
    ledVolume.setValue(volume);
    last_volume = volume;

    if (note != prev_note)
    {
        buzzer.play(note);
        ledGamme = rgb_led_color_selector[(note.gamme / 12) + 1];
    }

    prev_note = note;
}

#endif // __TEREMINE_H__