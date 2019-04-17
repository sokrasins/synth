/* Time.h
 * Interrupts located here
 * Stan Okrasinski
 */

#ifndef TIME_H
#define TIME_H

#include <stdint.h>

// Vars
extern volatile uint32_t msTicks; 

// Functions
void Delay (uint32_t dlyTicks);

#endif // TIME_H
