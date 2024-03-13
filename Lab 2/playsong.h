#ifndef __PLAYSONG_H
#define __PLAYSONG_H

#include "sys.h"
#include "buzzer.h"

uint32_t getNote(uint8_t ch);
uint32_t getDuration(uint8_t ch);
uint32_t getPause(uint8_t ch);
void playNote(uint32_t note, uint32_t durationMs);
void playSong(uint8_t *song);
#endif