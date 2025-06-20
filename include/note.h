#ifndef __NOTE_H__
#define __NOTE_H__

#include "mbed.h"
#include "wait.h"
#include <cmath>

#define DURATION_COEF 1

class Note
{

private:

    float freq;

public:

    enum Name
    {
        DO = -9,  // Semitones from LA4
        RE = -7,  // Semitones from LA4
        MI = -5,  // Semitones from LA4
        FA = -4,  // Semitones from LA4
        SOL = -2, // Semitones from LA4
        LA = 0,   // Reference note (A)
        SI = 2,   // Semitones from LA4
    };

    enum Gamme
    {
        G3 = -12, // Octave 3 (12 semitones below G4)
        G4 = 0,   // Reference octave 4
        G5 = +12  // Octave 5 (12 semitones above G4)
    };

    Name note;
    Gamme gamme;

    Note(Name n, Gamme g);
    float getFreq(void);
    void getName(char str[5]);

    bool operator==(const Note &other) const;
    bool operator!=(const Note &other) const;
    Note& operator=(const Note &other);

};

Note::Note(Name n, Gamme g) : note(n), gamme(g)
{
    freq = 440.0f * powf(2.0f, (float)(n + g) / 12.0f);
}

float Note::getFreq(void)
{
    return freq;
}

void Note::getName(char str[5])
{
    int gamme_idx = 2;

    if (!str)
        return;

    switch (note)
    {
        case DO:
            str[0] = 'D';
            str[1] = '0';
            break;
        case RE:
            str[0] = 'R';
            str[1] = 'E';
            break;
        case MI:
            str[0] = 'M';
            str[1] = 'I';
            break;
        case FA:
            str[0] = 'F';
            str[1] = 'A';
            break;
        case SOL:
            str[0] = 'S';
            str[1] = 'O';
            str[2] = 'L';
            gamme_idx = 3;
            break;
        case LA:
            str[0] = 'L';
            str[1] = 'A';
            break;
        case SI:
            str[0] = 'S';
            str[1] = 'I';
            break;
        default:
            break;
    }

    switch (gamme)
    {
        case G3:
            str[gamme_idx] = '3';
            break;
        case G4:
            str[gamme_idx] = '4';
            break;
        case G5:
            str[gamme_idx] = '5';
            break;
        default:
            break;
    }

    if (gamme_idx == 2)
        str[3] = ' ';

    return;
}

bool Note::operator==(const Note &other) const
{
    return (this->freq == other.freq);
}

bool Note::operator!=(const Note &other) const
{
    return (this->freq == other.freq);
}

Note& Note::operator=(const Note &other)
{
    this->note = other.note;
    this->gamme = other.gamme;
    this->freq = other.freq;
    return *this;
}

#endif // __NOTE_H__