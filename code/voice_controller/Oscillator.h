/* Oscillator.h
 * Code managing the oscillators
 * Stan Okrasinski
 */

#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <stdbool.h>
#include "DAC084S085.h"
#include "stm32f051x8.h"

#define SAMPLE_FREQ		96000UL
#define CPU_FREQ			48000000UL
#define MAX_FREQ			20000UL

// Types of waveforms available for each oscillator
typedef enum {
	WAVE_MANUAL = 0,	// UNTESTED
	WAVE_SQUARE = 1,
	WAVE_SINE = 2,		// TODO
	WAVE_NULL = 3,		// End of enum
} wave_type_t;

// Params required for square wave
typedef struct {
	uint16_t freq;
	uint16_t period;
	uint32_t nextLow;
	uint32_t nextHigh;
} square_config_t;

// Params required for manual
typedef struct {
	uint8_t level;
} manual_config_t;

// Let's all be friends
typedef union {
	manual_config_t manual;
	square_config_t square;
} wave_config_t;

// Defines what it takes to be an oscillator
typedef struct {
	dac_chan_t channel;
	wave_type_t wave_type;
	wave_config_t wave_config;
	bool enable;
} osc_config_t;

// ERR codes
// TODO bit-field a unified thing
typedef enum {
	ERROR_NONE = 0,
	INVALID_FREQ = 1,
	INVALID_WAVE_TYPE = 2,
	OSC_NUM_OUT_OF_BOUNDS = 3,
} osc_err_t;

// Functions
void oscillator_init(void);

// App API
osc_err_t set_osc_level(uint8_t osc_idx, uint8_t level);
osc_err_t set_osc_freq(uint8_t osc_idx, uint16_t freq);
osc_err_t set_osc_wave_type(uint8_t osc_idx, wave_type_t type);
osc_err_t set_osc_enable(uint8_t osc_idx, bool enable);

#endif // OSCILLATOR_H
