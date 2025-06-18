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

Note default_note(Note::LA, Note::G4, Note::UNSPECIFIED);

class Teremine
{

private:

    Capteur gamme;
    Capteur note;
    Capteur volume;
    Resistance resistance;
    Buzzer buzzer;
    LedVolume ledVolume;
    RGBLed ledGamme;

    const int capt_min_dist = 5;

    const int nb_note_per_gamme = 7;
    const int nb_gamme = 3;
    const int nb_volume = 32;

    const int dist_btw_note = 4;
    const int dist_btw_gamme = 10;

    bool is_first_volume_set = false;

    Note prev_note = default_note;

public:

    Teremine(Capteur gamme, Capteur note, Capteur volume, Resistance resistance, Buzzer buzzer, LedVolume ledVolume, RGBLed ledGamme);

    void cycle(void);
    void play_note(Note note, char volume);

};

Teremine::Teremine(Capteur gamme, Capteur note, Capteur volume, Resistance resistance, Buzzer buzzer, LedVolume ledVolume, RGBLed ledGamme)
    : gamme(gamme), note(note), volume(volume), resistance(resistance), buzzer(buzzer), ledVolume(ledVolume), ledGamme(ledGamme)
{}

void Teremine::cycle(void)
{

    long long note_dist = note.getDistance();
    long long gamme_dist = gamme.getDistance();
    long long volume_dist = volume.getDistance();
    int note_idx;
    int gamme_idx;
    int volume;
    Note::Name note_played;
    Note::Gamme gamme_played;
    char name[5] = {0};

    if (note_dist < capt_min_dist || note_dist > capt_min_dist + dist_btw_note * nb_note_per_gamme)
        note_dist = 0;
    if (gamme_dist < capt_min_dist || gamme_dist > capt_min_dist + dist_btw_gamme * nb_gamme)
        gamme_dist = 0;
    if (volume_dist < capt_min_dist || volume_dist > capt_min_dist + nb_volume)
        volume_dist = -1;

    if (note_dist && gamme_dist)
    {
        note_idx = (note_dist - capt_min_dist) / dist_btw_note;
        gamme_idx = (gamme_dist - capt_min_dist) / dist_btw_gamme;
        volume = volume_dist - capt_min_dist;

        switch (note_idx)
        {
            case 0:
                note_played = Note::DO;
                break;
            case 1:
                note_played = Note::RE;
                break;
            case 2:
                note_played = Note::MI;
                break;
            case 3:
                note_played = Note::FA;
                break;
            case 4:
                note_played = Note::SOL;
                break;
            case 5:
                note_played = Note::LA;
                break;
            case 6:
                note_played = Note::SI;
                break;
        }

        switch (gamme_idx)
        {
            case 0:
                gamme_played = Note::G3;
                break;
            case 1:
                gamme_played = Note::G4;
                break;
            case 2:
                gamme_played = Note::G5;
                break;
        }

        Note curr_note(note_played, gamme_played, Note::UNSPECIFIED);

        play_note(curr_note, volume);
        curr_note.getName(name);

        printf(">> %.5s @ %02d [ n: %02lld cm | g: %02lld cm | v: %02lld cm ]\n", name, volume, note_dist, gamme_dist, volume_dist);

    }
    else
        buzzer.mute();

    wait_ms(250);

    return;
}

void Teremine::play_note(Note note, char volume)
{
    if (is_first_volume_set && volume >= 0 && volume <= 32)
    {
        resistance.setValue(volume);
        ledVolume.setValue(volume);
    }
    else if (!is_first_volume_set)
    {
        resistance.setValue(25);
        ledVolume.setValue(25);
        is_first_volume_set = true;
    }

    if (note.getFreq() != prev_note.getFreq())
    {
        if (note.duration == Note::UNSPECIFIED)
        {
            buzzer.play(note);

            switch (note.gamme)
            {
                case Note::G3:
                    ledGamme.setColor(RGBLed::RED);
                    break;
                case Note::G4:
                    ledGamme.setColor(RGBLed::GREEN);
                    break;
                case Note::G5:
                    ledGamme.setColor(RGBLed::BLUE);
                    break;
            }

            wait_ms(Note::NOIRE);
        }
        else
        {
            buzzer.play(note);

            switch (note.gamme)
            {
                case Note::G3:
                    ledGamme.setColor(RGBLed::RED);
                    break;
                case Note::G4:
                    ledGamme.setColor(RGBLed::GREEN);
                    break;
                case Note::G5:
                    ledGamme.setColor(RGBLed::BLUE);
                    break;
            }
            
            wait_ms(note.duration);
        }
    }
    else
    {
        if (note.duration == Note::UNSPECIFIED)
            wait_ms(Note::NOIRE);
        else
            wait_ms(note.duration);
    }
}

#endif // __TEREMINE_H__