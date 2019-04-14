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

// Main

int main (void) {

	device_init();
	oscillator_init();
	
	set_osc_freq(0, 110);
	set_osc_freq(1, 220);
	set_osc_freq(2, 440);
	set_osc_freq(3, 880);
	
	// Main loop
  for (;;) {

  }
}
