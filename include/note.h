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
        RE = -7,
        MI = -5,
        FA = -4,
        SOL = -2,
        LA = 0,
        SI = 2,
    };

    enum Duration
    {
        UNSPECIFIED = 0,
        DOUBLE_CROCHE = 250 * DURATION_COEF,
        CROCHE = 500 * DURATION_COEF,
        CROCHE_POINTE = 750 * DURATION_COEF,
        NOIRE = 1000 * DURATION_COEF,
        NOIRE_POINTE = 1500 * DURATION_COEF,
        BLANCHE = 2000 * DURATION_COEF,
        BLANCHE_POINTE = 3000 * DURATION_COEF,
        RONDE = 4000 * DURATION_COEF
    };

    enum Gamme
    {
        G3 = -12,
        G4 = 0,
        G5 = +12
    };

    const Name note;
    const Gamme gamme;
    const Duration duration;

    Note(Name n, Gamme g, Duration d);
    float getFreq(void);
    void getName(char str[5]);

};

Note::Note(Name n, Gamme g, Duration d) : note(n), gamme(g), duration(d)
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
            str[1] = '0';
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
    }

    if (gamme_idx == 2)
        str[3] = ' ';

    return;
}

#endif // __NOTE_H__