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
	
	// Main loop
  for (;;) {
		set_osc_level(1, 0xFF);
		Delay(1);
		
		set_osc_level(1, 0x00);
		Delay(1);
  }
}
