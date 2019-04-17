/* Time.c
 * Timing functions go here
 * Stan Okrasinski
 */

#include "stm32f0xx.h"
#include "Time.h"

// incremented every ms
volatile uint32_t msTicks;

void SysTick_Handler(void) {
  msTicks++;
}

// Delay milliseconds (blocking)
void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;
  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) { __NOP(); }
}
