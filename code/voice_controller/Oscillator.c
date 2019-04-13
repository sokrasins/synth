#include "Oscillator.h"

#include <string.h>

#include "DAC084S085.h"
#include "Board_LED.h"  

// Local functions
void sample_clock_init(void);

// Vars
static const dac_chan_t osc_dict[NUM_CHANNELS] = {CHAN_A, CHAN_B, CHAN_C, CHAN_D};
static uint16_t chan_data[NUM_CHANNELS];

void oscillator_init(void) {
	// Config DAC and osc
	dac_init();
	sample_clock_init();
	
	// Init variables
	memset((void *)chan_data, 0x00, NUM_CHANNELS);
}

void sample_clock_init(void) {
	// Enable clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;

	// Enable update events
	TIM14->DIER = TIM_DIER_UIE;
	
	// Set timebase. ARR of 500 => 96 kHz
	// sample clock
	TIM14->PSC = 0UL;
	TIM14->ARR = 500UL;
	
	// Set IRQ priority and enable
	NVIC_SetPriority(TIM14_IRQn, 1);
	NVIC_EnableIRQ(TIM14_IRQn);
	
	// Enable Timer
	TIM14->CR1 = TIM_CR1_CEN;
}

void set_osc_level(uint8_t osc, uint8_t level) {
	chan_data[osc_dict[osc]] = (level << DATA_OFFSET) | 
		(WRITE_REG_DO_UPDATE << OPERATION_OFFSET) | 
		(chan_data[osc] << CHANNEL_OFFSET);
}

// Oscillator sample clock
// Responsible for writing osc values to the DAC
void TIM14_IRQHandler(void) {
	uint8_t i;
	
	// Clear interrupt flag
	if (TIM14->SR & TIM_SR_UIF) {
		TIM14->SR &= ~TIM_SR_UIF;
	}
	
	// Write all DAC values
	for (i=0; i<NUM_CHANNELS; i++) {
		set_dac_channel(chan_data[i]);
	}
}
