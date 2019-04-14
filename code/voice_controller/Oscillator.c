#include "Oscillator.h"

#include <string.h>
#include <stdbool.h>

#include "DAC084S085.h"
#include "Board_LED.h"  

// Local functions
void sample_clock_init(void);

// Vars
static osc_config_t osc[NUM_CHANNELS];
static uint32_t sample_clock = 0UL; 

void oscillator_init(void) {
	uint8_t i;

	// Init all oscillators as square gens of 0 Hz
	for (i=0; i<NUM_CHANNELS; i++) {
		osc[i].channel = (dac_chan_t)i;
		osc[i].wave_type = WAVE_NULL;
		osc[i].enable = false;
	}
	
	// Init DAC and sample timer
	dac_init();
	sample_clock_init();
}

void sample_clock_init(void) {
	// Enable clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;

	// Enable update events
	TIM14->DIER = TIM_DIER_UIE;
	
	// Set timebase of sample clock
	TIM14->PSC = 0UL;
	TIM14->ARR = CPU_FREQ/SAMPLE_FREQ;
	
	// Set IRQ priority and enable
	NVIC_SetPriority(TIM14_IRQn, 1);
	NVIC_EnableIRQ(TIM14_IRQn);
	
	// Enable Timer
	TIM14->CR1 = TIM_CR1_CEN;
}

// Need to follow up with configuration calls to turn on
osc_err_t set_osc_wave_type(uint8_t osc_idx, wave_type_t type) {
	if (osc_idx >= NUM_CHANNELS) {
		return OSC_NUM_OUT_OF_BOUNDS;
	}
	
	if (type >= WAVE_NULL) {
		return INVALID_WAVE_TYPE;
	}
	
	if (osc[osc_idx].wave_type == type || 
			osc[osc_idx].wave_type == WAVE_SINE) {
		return INVALID_WAVE_TYPE;
	}
	
	osc[osc_idx].enable = false;
	osc[osc_idx].wave_type = type;
	
	switch(type) {
		case WAVE_MANUAL:
			osc[osc_idx].wave_config.manual.level = 0x00;
			break;
		case WAVE_SQUARE:
			osc[osc_idx].wave_config.square.freq = 0x00;
			osc[osc_idx].wave_config.square.nextHigh = 0xFFFFFFFF;
			osc[osc_idx].wave_config.square.nextLow = 0xFFFFFFFF;
			osc[osc_idx].wave_config.square.period = 0xFFFF;
			break;
		case WAVE_SINE:
		case WAVE_NULL:
		default:
			return INVALID_WAVE_TYPE;
	}
	
	return ERROR_NONE;
}

osc_err_t set_osc_enable(uint8_t osc_idx, bool enable) {
	if (osc_idx >= NUM_CHANNELS) {
		return OSC_NUM_OUT_OF_BOUNDS;
	}
	
	osc[osc_idx].enable = enable;
	return ERROR_NONE;
}

osc_err_t set_osc_freq(uint8_t osc_idx, uint16_t freq) {
	square_config_t *squ;
	
	// Is the oscillator index valid?
	if (osc_idx >= NUM_CHANNELS) {
		return OSC_NUM_OUT_OF_BOUNDS;
	}
	
	// Is the frequency valid?
	if (freq > MAX_FREQ) {
		return INVALID_FREQ;
	}
	
	// Check if the wave type has a relevant parameter
	if (!(osc[osc_idx].wave_type == WAVE_SQUARE || 
			osc[osc_idx].wave_type == WAVE_SINE)) {
		return INVALID_WAVE_TYPE;
	}
		
	// Set the frequency parameter accordingly
	if (osc[osc_idx].wave_type == WAVE_SQUARE) {
		squ = &osc[osc_idx].wave_config.square;
		
		// Config square params
		squ->freq = freq;
		squ->period = SAMPLE_FREQ/freq;
		squ->nextLow = sample_clock + squ->period/2;
		squ->nextHigh = sample_clock + squ->period;
		
	} else if (osc[osc_idx].wave_type == WAVE_SINE) {
		return INVALID_WAVE_TYPE;
	}
	
	return ERROR_NONE;
}

osc_err_t set_osc_level(uint8_t osc_idx, uint8_t level) {
	// Is the oscillator index valid?
	if (osc_idx >= NUM_CHANNELS) {
		return OSC_NUM_OUT_OF_BOUNDS;
	}
	
	// NOTE: TRIPPINESS POSSIBLE BY CO-MINGLING
	// Check if osc is the right wave type
	if (osc[osc_idx].wave_type != WAVE_MANUAL) {
		return INVALID_WAVE_TYPE;
	}
	
	// Set the osc level
	osc[osc_idx].wave_config.manual.level = level;
	return ERROR_NONE;
}

// Oscillator sample clock
// Responsible for writing osc values to the DAC
void TIM14_IRQHandler(void) {
	uint8_t i;
	
	// Increment global sample clock
	sample_clock++;
	
	// Clear interrupt flag
	if (TIM14->SR & TIM_SR_UIF) {
		TIM14->SR &= ~TIM_SR_UIF;
	}
	
	// Write all DAC values according to wave type
	for (i=0; i<NUM_CHANNELS; i++) {
		if (osc[i].enable || osc[i].wave_type != WAVE_NULL) {
			switch (osc[i].wave_type) {
				case WAVE_MANUAL:
					set_dac_channel(osc[i].channel, osc[i].wave_config.manual.level);
					break;
			
				case WAVE_SQUARE:
					if (osc[i].wave_config.square.nextLow <= sample_clock) {
						osc[i].wave_config.square.nextLow += osc[i].wave_config.square.period;
						set_dac_channel(osc[i].channel, 0x00);
					} else if (osc[i].wave_config.square.nextHigh <= sample_clock) {
						osc[i].wave_config.square.nextHigh += osc[i].wave_config.square.period;
						set_dac_channel(osc[i].channel, 0xFF);
					}
					break;
			
				case WAVE_SINE:
				default:
					break;
			}
		}
	}
}
