/* Control.h
 * Code managing the oscillator effects
 * Stan Okrasinski
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
} pwm_config_t;

void control_init(void);
ctrl_err_t set_vca_level(uint8_t idx, uint16_t level);

#endif // CONTROL_H
