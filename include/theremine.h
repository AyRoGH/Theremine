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

#define MESURE_MOYENNE 1

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

    const int capt_min_dist = 5;
    const int nb_note_per_gamme = 7;
    const int nb_gamme = 3;
    const int nb_volume = 32;

    const int dist_btw_note = 4;
    const int dist_btw_gamme = 10;

    bool is_first_volume_set = false;

    static const Note default_note;

    Note prev_note = default_note;

    static const Note::Name note_name_selector[7];
    static const Note::Gamme note_gamme_selector[3];
    static const RGBLed::color rgb_led_color_selector[3];

    long long getDistance(Capteur capt);

public:

    Theremine(Capteur gamme, Capteur note, Capteur volume, Resistance resistance, Buzzer buzzer, LedVolume ledVolume, RGBLed ledGamme);

    void cycle(void);
    void play_note(Note note, unsigned char volume);

};

const Note Theremine::default_note(Note::LA, Note::G4, Note::UNSPECIFIED);

const Note::Name Theremine::note_name_selector[7] = {Note::DO, Note::RE, Note::MI, Note::FA, Note::SOL, Note::LA, Note::SI};
const Note::Gamme Theremine::note_gamme_selector[3] = {Note::G3, Note::G4, Note::G5};
const RGBLed::color Theremine::rgb_led_color_selector[3] = {RGBLed::RED, RGBLed::GREEN, RGBLed::BLUE};

Theremine::Theremine(Capteur gamme, Capteur note, Capteur volume, Resistance resistance, Buzzer buzzer, LedVolume ledVolume, RGBLed ledGamme)
    : gammeCapt(gamme), noteCapt(note), volumeCapt(volume), resistance(resistance), buzzer(buzzer), ledVolume(ledVolume), ledGamme(ledGamme)
{}

long long Theremine::getDistance(Capteur capt)
{
    long long measure1 = capt.getDistance();
    wait_ms(25);
    long long measure2 = capt.getDistance();
    wait_ms(25);
    long long measure3 = capt.getDistance();

    return (measure1 + measure2 + measure3) / 3;
}

void Theremine::cycle(void)
{
    #if MESURE_MOYENNE == 0
    long long note_dist = noteCapt.getDistance();
    long long gamme_dist = gammeCapt.getDistance();
    long long volume_dist = volumeCapt.getDistance();
    #else
    long long note_dist = this->getDistance(noteCapt);
    long long gamme_dist = this->getDistance(gammeCapt);
    long long volume_dist = this->getDistance(volumeCapt);
    #endif
    int note_idx;
    int gamme_idx;
    unsigned char volume;
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
        volume = volume_dist >= capt_min_dist ? volume_dist - capt_min_dist : nb_volume;

        note_played = note_name_selector[note_idx];
        gamme_played = note_gamme_selector[gamme_idx];

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

void Theremine::play_note(Note note, unsigned char volume)
{
    if (is_first_volume_set && volume < nb_volume)
    {
        resistance.setValue(volume);
        ledVolume.setValue(volume);
    }
    else if (!is_first_volume_set)
    {
        resistance.setValue(25); // Default value
        ledVolume.setValue(25); // Default value
        is_first_volume_set = true;
    }

    bool note_changed = (note != prev_note);

    if (note_changed) {
        buzzer.play(note);
        ledGamme = rgb_led_color_selector[(note.gamme / 12) + 1];
    }

    if (note.duration == Note::UNSPECIFIED)
        wait_ms(Note::NOIRE);
    else
        wait_ms(note.duration);

    prev_note = note;
}

#endif // __TEREMINE_H__