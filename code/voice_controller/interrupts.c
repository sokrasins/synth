/* interrupts.h
 * Interrupts located here
 * Stan Okrasinski
 */

#include "stm32f0xx.h"
#include "interrupts.h"
#include <stdbool.h>

volatile uint32_t msTicks;              // counts 1ms timeTicks

void SysTick_Handler(void) {
  msTicks++;
}

// Delay milliseconds (blocking)
void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;
  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) { __NOP(); }
}
