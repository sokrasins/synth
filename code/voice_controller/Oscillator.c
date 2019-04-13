#include "Oscillator.h"

#include "DAC084S085.h"

// Vars
static uint8_t chan_data[NUM_CHANNELS];
static const dac_chan_t osc_dict[NUM_CHANNELS] = {CHAN_A, CHAN_B, CHAN_C, CHAN_D};

void oscillator_init(void) {
	dac_init();
}

void set_osc_level(uint8_t osc, uint8_t level) {
	chan_data[osc_dict[osc]] = level;
	
	// debug
	set_dac_channel(osc_dict[osc], level);
}

