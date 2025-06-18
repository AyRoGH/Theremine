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

    const int capt_min_dist = 5;        // Minimum reliable distance for sensors (cm)
    const int nb_note_per_gamme = 7;    // Number of notes in one octave/gamme (Do, Re, Mi, Fa, Sol, La, Si)
    const int nb_gamme = 3;             // Number of available gammes/octaves
    const int nb_volume = 32;           // Max value for volume mapping (0-31 for resistance)

    const int dist_btw_note = 4;        // Distance range per note (cm)
    const int dist_btw_gamme = 10;      // Distance range per gamme/octave (cm)
    // Note: Max note distance = capt_min_dist + dist_btw_note * nb_note_per_gamme
    // Note: Max gamme distance = capt_min_dist + dist_btw_gamme * nb_gamme
    // Note: Max volume distance = capt_min_dist + nb_volume (this seems to be the logic from cycle())

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

    // Consider adding a filter (e.g., moving average) to stabilize sensor readings
    long long note_dist = note.getDistance();
    // Consider adding a filter (e.g., moving average) to stabilize sensor readings
    long long gamme_dist = gamme.getDistance();
    // Consider adding a filter (e.g., moving average) to stabilize sensor readings
    long long volume_dist = volume.getDistance();
    int note_idx;
    int gamme_idx;
    int volume;
    Note::Name note_played;
    Note::Gamme gamme_played;
    char name[5] = {0};

    // Validate sensor readings and map to specific behaviors if out of expected range
    if (note_dist < capt_min_dist || note_dist > capt_min_dist + dist_btw_note * nb_note_per_gamme)
        note_dist = 0; // Treat out-of-range for note as no note detected
    if (gamme_dist < capt_min_dist || gamme_dist > capt_min_dist + dist_btw_gamme * nb_gamme)
        gamme_dist = 0; // Treat out-of-range for gamme as no gamme detected
    if (volume_dist < capt_min_dist || volume_dist > capt_min_dist + nb_volume)
        volume_dist = -1; // Treat out-of-range for volume as invalid/unreliable

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
            default:
                // Invalid note_idx, play a default note
                note_played = Note::LA;
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
            default:
                // Invalid gamme_idx, play a default gamme
                gamme_played = Note::G4;
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

    bool note_changed = (note.getFreq() != prev_note.getFreq());

    if (note_changed) {
        buzzer.play(note);
        switch (note.gamme) {
            case Note::G3:
                ledGamme.setColor(RGBLed::RED);
                break;
            case Note::G4:
                ledGamme.setColor(RGBLed::GREEN);
                break;
            case Note::G5:
                ledGamme.setColor(RGBLed::BLUE);
                break;
            // Consider adding a default case for ledGamme or ensuring all gammes are covered
        }
    }

    if (note.duration == Note::UNSPECIFIED) {
        wait_ms(Note::NOIRE);
    } else {
        wait_ms(note.duration);
    }

    prev_note = note;
}

#endif // __TEREMINE_H__