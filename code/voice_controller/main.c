/*
 * By Stan Okrasinski
 *
 * Peripheral Usage:
 * - TIM14: Sample Clock
 * - SPI1: DAC communication
		 - PA4, PA5, PA7
 */

#include "InitDevice.h"
#include "stm32f0xx.h"

#include "interrupts.h"
#include "Oscillator.h"
#include "Control.h"

// Main
int main (void) {
	uint8_t i;
	uint16_t j;	
	uint16_t base_freq = 55;
	
	// Component initialization
	device_init();
	oscillator_init();
	control_init();
	
	// Oscillator setup
	for (i=0; i<4; i++) {
		set_osc_wave_type(i, WAVE_SQUARE);
		set_osc_freq(i, base_freq*(i+1));
		set_osc_enable(i, true);
	}
	
	// Main loop
  for (;;) {
		for(i=0; i<8; i++) {
			set_osc_freq(0, base_freq*(1UL<<i));
			for(j=0; j<655; j++) {
				set_vca_level(0, j*100);
				Delay(1);
			}
		}
  }
}
