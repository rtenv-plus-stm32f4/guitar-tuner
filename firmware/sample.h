#ifndef SAMPLE_H
#define SAMPLE_H

#define ADC3_DR_ADDRESS     ((uint32_t)0x4001224C)

void USART1_puts(char* s);

void init_rs232(void);

void TIM3_config(void);

#endif
