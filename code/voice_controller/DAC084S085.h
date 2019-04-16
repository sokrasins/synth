/* DAC084S085.h
 * Driver for 8-bit TI DAC
 * Stan Okrasinski
 *
 * Peripheral usage:
 *
 * SPI1 - DAC comm
 * - PA04: NSS
 * - PA05: SCK
 * - PA07: MOSI
 */

#ifndef DAC084S085_H
#define DAC084S085_H

#include <stdint.h>

// Defines
#define NUM_CHANNELS 4

#define DATA_OFFSET 4
#define OPERATION_OFFSET 12
#define CHANNEL_OFFSET 14 

// Types
typedef enum dac_op_e {
	WRITE_REG_DONT_UPDATE = 0x0,
	WRITE_REG_DO_UPDATE = 0x1,
	WRITE_ALL_DO_UPDATE = 0x2,
	POWER_DOWN = 0x3
} dac_op_t;

typedef enum dac_chan_e {
	CHAN_A = 0x0,
	CHAN_B = 0x1,
	CHAN_C = 0x2,
	CHAN_D = 0x3
} dac_chan_t;

// Functions
void dac_init(void);
void set_dac_channel(dac_chan_t chan, uint8_t val);

#endif // DAC084S085_H
