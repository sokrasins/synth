/*
 * By Stan Okrasinski
 */

#include "InitDevice.h"
#include "stm32f0xx.h"

#include "Time.h"
#include "Oscillator.h"
#include "Control.h"

void led_init(void);

// Main
int main (void) {
	uint8_t i=0;
	uint16_t j;	
	uint16_t base_freq = 55;
	
	// Component initialization
	device_init();
	oscillator_init();
	control_init();
	led_init();
	
	// Oscillator setup
	for (i=0; i<4; i++) {
		set_osc_wave_type(i, WAVE_SQUARE);
		set_osc_freq(i, base_freq*(i+1));
		set_osc_enable(i, true);
	}
	
	set_osc_freq(0, 1000UL);
	set_filt_cutoff(0, 0x8000);
	
	// Main loop
  for (;;) {
		for(j=0; j<655; j++) {
			//set_vca_level(0, j*100);
			set_filt_cutoff(0, j*100);
			Delay(1);
		}
  }
}

void led_init(void) {
	GPIOA->OTYPER &= ~(1UL<<15);															// LED pin as push-pull
	GPIOA->MODER |= 1UL<<30;																	// LED pin as output
	GPIOA->BSRR |= 1UL<<15;																	  // Turn LED on
}
