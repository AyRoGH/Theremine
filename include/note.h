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
        DO = -9,
        DO_DIEZE = -8,
        RE = -7,
        RE_DIEZE = -6,
        MI = -5,
        FA = -4,
        FA_DIEZE = -3,
        SOL = -2,
        SOL_DIEZE = -1,
        LA = 0,
        LA_DIEZE = 1,
        SI = 2
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

void Note::getName(char str[6])
{
    int gamme_idx = 3;

    if (!str)
        return;

    switch (note)
    {
        case DO:
            str[0] = 'D';
            str[1] = '0';
            str[2] = ' ';
            break;
        case DO_DIEZE:
            str[0] = 'D';
            str[1] = '0';
            str[2] = '#';
            break;
        case RE:
            str[0] = 'R';
            str[1] = 'E';
            str[2] = ' ';
            break;
        case RE_DIEZE:
            str[0] = 'R';
            str[1] = 'E';
            str[2] = '#';
            break;
        case MI:
            str[0] = 'M';
            str[1] = 'I';
            str[2] = ' ';
            break;
        case FA:
            str[0] = 'F';
            str[1] = 'A';
            str[2] = ' ';
            break;
        case FA_DIEZE:
            str[0] = 'F';
            str[1] = 'A';
            str[2] = '#';
            break;
        case SOL:
            str[0] = 'S';
            str[1] = 'O';
            str[2] = 'L';
            str[3] = ' ';
            gamme_idx = 4;
            break;
        case SOL_DIEZE:
            str[0] = 'S';
            str[1] = 'O';
            str[2] = 'L';
            str[3] = '#';
            gamme_idx = 4;
            break;
        case LA:
            str[0] = 'L';
            str[1] = 'A';
            str[2] = ' ';
            break;
        case LA_DIEZE:
            str[0] = 'L';
            str[1] = 'A';
            str[2] = '#';
            break;
        case SI:
            str[0] = 'S';
            str[1] = 'I';
            str[2] = ' ';
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

    if (gamme_idx == 3)
        str[4] = ' ';

    return;
}

bool Note::operator==(const Note &other) const
{
    return (this->freq == other.freq);
}

bool Note::operator!=(const Note &other) const
{
    return (this->freq != other.freq);
}

Note& Note::operator=(const Note &other)
{
    this->note = other.note;
    this->gamme = other.gamme;
    this->freq = other.freq;
    return *this;
}

#endif // __NOTE_H__