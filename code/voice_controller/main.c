/*
 * By Stan Okrasinski
 */

#include "InitDevice.h"
#include "interrupts.h"
#include "Oscillator.h"

#include "stm32f0xx.h"
#include <stdbool.h>
#include "Board_LED.h"
#include "DAC084S085.h"


#define FREQ	8500
#define TICK_FREQ 400000

// Functions

// Delay milliseconds (blocking)
void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;
  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) { __NOP(); }
}

// Main

int main (void) {

	device_init();
	oscillator_init();
	
	// Main loop
  for (;;) {
		set_osc_level(1, 0xFF);
		Delay(1);
		
		set_osc_level(1, 0x00);
		Delay(1);
  }
}
