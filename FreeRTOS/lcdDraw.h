#ifndef LCDDRAW_H
#define LCDDRAW_H


#include "em_device.h"
#include "em_chip.h"
#include "segmentlcd.h"

#include "../application/segmentlcd_individual.h"


void lcdDrawBasket(uint8_t position); //Kos�r kijelz�se LCD-n

void lcdDrawScore(uint8_t score, uint8_t round); //K�r�k sz�ma �s pontsz�m kijelz�se LCD-n

void lcdDrawBanana(uint8_t position, uint8_t age); //Ban�n kijelz�se LCD-n

void lcdClearLowerSegment(); //Als� szegmes t�rl�se


#endif
