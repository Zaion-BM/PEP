#ifndef LCDDRAW_H
#define LCDDRAW_H


#include "em_device.h"
#include "em_chip.h"
#include "segmentlcd.h"

#include "../application/segmentlcd_individual.h"


void lcdDrawBasket(uint8_t position); //Kosár kijelzése LCD-n

void lcdDrawScore(uint8_t score, uint8_t round); //Körök száma és pontszám kijelzése LCD-n

void lcdDrawBanana(uint8_t position, uint8_t age); //Banán kijelzése LCD-n

void lcdClearLowerSegment(); //Alsó szegmes törlése


#endif
