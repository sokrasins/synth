/* Control.h
 * Code managing the oscillator effects
 * Stan Okrasinski
 */

#include "Control.h"

#include "stm32f051x8.h"
#include "DAC084S085.h"

voice_ctrl_t ctrl[NUM_CHANNELS];

void timer_init(pwm_config_t *pwm);

// Init all PWM's
void control_init(void) {
	pwm_config_t config;
	
	// Configure Timer 1
	config.gpio = GPIOA;
	config.tim = TIM1;
	config.pins[0] = 8;
	config.pins[1] = 9;
	config.pins[2] = 10;
	config.pins[3] = 11;
	
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	timer_init(&config);
	
	// Configure Timer 2
	config.gpio = GPIOA;
	config.tim = TIM2;
	config.pins[0] = 0;
	config.pins[1] = 1;
	config.pins[2] = 2;
	config.pins[3] = 3;
	
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	timer_init(&config);
	
	// Configure Timer 3
	config.gpio = GPIOC;
	config.tim = TIM3;
	config.pins[0] = 6;
	config.pins[1] = 7;
	config.pins[2] = 8;
	config.pins[3] = 9;
	
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	timer_init(&config);
	
	// Fill up voice control groups
	ctrl[0].vca_pwm_reg = &TIM1->CCR1;
	ctrl[0].filt_f_pwm_reg = &TIM1->CCR2;
	ctrl[0].filt_q_pwm_reg = &TIM1->CCR3;
	
	ctrl[1].vca_pwm_reg = &TIM1->CCR4;
	ctrl[1].filt_f_pwm_reg = &TIM2->CCR1;
	ctrl[1].filt_q_pwm_reg = &TIM2->CCR2;
	
	ctrl[2].vca_pwm_reg = &TIM2->CCR3;
	ctrl[2].filt_f_pwm_reg = &TIM2->CCR4;
	ctrl[2].filt_q_pwm_reg = &TIM3->CCR1;
	
	ctrl[3].vca_pwm_reg = &TIM3->CCR2;
	ctrl[3].filt_f_pwm_reg = &TIM3->CCR3;
	ctrl[3].filt_q_pwm_reg = &TIM3->CCR4;
}

void timer_init(pwm_config_t *pwm) {
	uint8_t i;
	
	// Configure GPIO to alternate function
	for (i=0; i<NUM_PINS; i++) {
		pwm->gpio->MODER |= 2<<(pwm->pins[i]*2);
	}
	
	// Set pins to AF2
	for(i=0; i<NUM_PINS; i++) {
		pwm->gpio->AFR[pwm->pins[i]>>3] |= (0x02<<(4*(pwm->pins[i]-8*(pwm->pins[i]>>3))));
	}
	
	// Set up timebase
	pwm->tim->PSC = 0UL;
	pwm->tim->ARR = PWM_RELOAD;
	
	// Set Initial PWM vals
	pwm->tim->CCR1 = 120;		// 25%
	pwm->tim->CCR2 = 240;		// 50%
	pwm->tim->CCR3 = 360;		// 75%
	pwm->tim->CCR4 = 479;		// 99%
	
	pwm->tim->CCMR1 = TIM_CCMR1_OC1PE | 			// Preload OC 1 regs
		TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 |		// Set OC1 to PWM mode 1
		TIM_CCMR1_OC2PE | 											// Preload OC 2 regs
		TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;		// Set OC2 to PWM mode 1
	
	pwm->tim->CCMR2 = TIM_CCMR2_OC3PE | 			// Preload OC 3 regs
		TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2 |		// Set OC3 to PWM mode 1
		TIM_CCMR2_OC4PE | 											// Preload OC 4 regs
		TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2;		// Set OC4 to PWM mode 1
	
	pwm->tim->CCER = TIM_CCER_CC1E | 					// Enable OC1
		TIM_CCER_CC2E |													// Enable OC2
		TIM_CCER_CC3E | 												// Enable OC3
		TIM_CCER_CC4E;													// Enable OC 4
	
	pwm->tim->BDTR |= TIM_BDTR_MOE;						// Enable output
	pwm->tim->CR1 |= TIM_CR1_CEN;							// Enable timer
	pwm->tim->EGR |= TIM_EGR_UG;							// Load in new reg values
}

ctrl_err_t set_vca_level(uint8_t idx, uint16_t level) {
	// Check for valid voice #
	if (idx >= NUM_CHANNELS) {
		return CTRL_NUM_OUT_OF_BOUNDS;
	}
	
	// Set register value, scaling for max pwm value
	*ctrl[idx].vca_pwm_reg = (level*PWM_RELOAD)/0xFFFF;
	
	// Just go ahead and update all timers because lazy
	// TODO: Update the RIGHT timer
	TIM1->EGR |= TIM_EGR_UG;
	TIM2->EGR |= TIM_EGR_UG;
	TIM3->EGR |= TIM_EGR_UG;
	
	return CTRL_ERROR_NONE;
}
