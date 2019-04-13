#include "DAC084S085.h"

#include "stm32f051x8.h"

// Functions
void _dac_write(uint16_t msg);

void dac_init(void) {
	// Enable SPI and GPIO clocks
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	RCC->AHBENR  |= RCC_AHBENR_GPIOAEN;
	
	// Set up NSS pin (PA4). AFR is 0, so don't need to set it manually
	GPIOA->OTYPER &= ~(1UL<<4);	// Set pin as push-pull
	GPIOA->MODER |= 2UL<<(2*4); // Set pin as alternate function
	
	// Set up SCK pin (PA5). AFR is 0, so don't need to set it manually
	GPIOA->OTYPER &= ~(1UL<<5); // Set pin as push-pull
	GPIOA->MODER |= 2UL<<(2*5);	// Set pin as alternate function
	
	// Set up MOSI pin (PA7). AFR is 0, so don't need to set it manually
	GPIOA->OTYPER &= ~(1UL<<7); // Set pin as push-pull
	GPIOA->MODER |= 2UL<<(2*7); // Set pin as alternate function
	
	// Set as master.
	// CPOL and CPHA not used in TI mode, don't need to set them
	SPI1->CR1 = SPI_CR1_MSTR;
	
	// Set TI mode (uses NSS as frame sync)
	// Set frame size to 16 bit
	SPI1->CR2 = SPI_CR2_FRF | SPI_CR2_DS;

	// Enable SPI
	SPI1->CR1 |= SPI_CR1_SPE;
}

// Write properly-formatted data to DAC
void _dac_write(uint16_t msg) {
	SPI1->DR = msg;
}

// Application interface, set the next value of the DAC to be written.
void set_dac_channel(dac_chan_t chan, uint8_t data) {
	uint16_t msg = 0x0000;
	
	msg = (data << DATA_OFFSET) | (WRITE_REG_DO_UPDATE << OPERATION_OFFSET) | (chan << CHANNEL_OFFSET); 
	_dac_write(msg);
}
