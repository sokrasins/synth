/* Control.h
 * Code managing the oscillator effects
 * Stan Okrasinski
 *
 * Peripheral usage:
 * 
 * TIM1: PWM generation - no interrupts
 * - PA08: osc 0, VCA
 * - PA09: osc 0, filt f
 * - PA10: osc 0, filt q
 * - PA11: osc 1, VCA
 *
 * TIM2: PWM generation - no interrupts
 * - PA00: osc 1, filt f
 * - PA01: osc 1, filt q
 * - PA02: osc 2, VCA
 * - PA03: osc 2, filt f
 *
 * TIM3: PWM generation - no interrupts
 * - PC06: osc 2, filt q  // TODO: Change to PB4
 * - PC07: osc 3, VCA     // TODO: Change to PB5
 * - PC08: osc 3, filt f  // TODO: Change to PB0
 * - PC09: osc 3, filt q  // TODO: Change to PB1
 */
 

#ifndef CONTROL_H
#define CONTROL_H

#include "stm32f051x8.h"

#define NUM_PINS		4
#define PWM_RELOAD	480UL	// 100kHz pwm freq

// ERR codes
// TODO bit-field a unified thing
typedef enum {
	CTRL_ERROR_NONE = 0,
	CTRL_NUM_OUT_OF_BOUNDS = 3,
} ctrl_err_t;

typedef struct {
	__IO uint32_t *vca_pwm_reg;
	__IO uint32_t *filt_f_pwm_reg;
	__IO uint32_t *filt_q_pwm_reg;
} voice_ctrl_t;

typedef struct {
	TIM_TypeDef *tim;
	GPIO_TypeDef *gpio;
	uint8_t pins[NUM_PINS]; 
	uint8_t afr;
} pwm_config_t;

void control_init(void);
ctrl_err_t set_vca_level(uint8_t idx, uint16_t level);
ctrl_err_t set_filt_cutoff(uint8_t idx, uint16_t level);

#endif // CONTROL_H
