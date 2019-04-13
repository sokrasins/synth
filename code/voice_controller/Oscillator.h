/* Oscillator.h
 * Code managing the oscillators
 * Stan Okrasinski
 */

#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include "stm32f051x8.h"
#include "DAC084S085.h"

void oscillator_init(void);
void set_osc_level(uint8_t chan, uint8_t level);

void TIM14_IRQHandler(void);

#endif
