#include "mbed.h"
#include "include/capteur.h"
#include "include/buzzer.h"
#include "include/resistance.h"
#include "include/led_volume.h"
#include "include/rgb_led.h"
#include "include/theremine.h"
#include "include/wait.h"

Capteur noteCapt(p8, p7);
Capteur gammeCapt(p10, p9);
Capteur volumeCapt(p6, p5);
Buzzer buzzer(p21);
Resistance resistance(p22, p24, p23);
LedVolume ledVolume(p15, p16, p17, p18, p19, p20);
RGBLed ledGamme(p25, p26, p27);
Theremine theremine(gammeCapt, noteCapt, volumeCapt, resistance, buzzer, ledVolume, ledGamme);

int main(void)
{

    printf("\n===================\n\n");

    ledGamme = RGBLed::WHITE;

    while (true)
    {
        theremine.cycle();
    }
}