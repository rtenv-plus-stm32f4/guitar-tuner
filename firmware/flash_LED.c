#include <stm32f4xx_gpio.h>
#include "flash_LED.h"
#include "syscall.h"

/* LED3: PG13, LED4: PG14 */
void init_GPIO_LED()
{
	GPIO_InitTypeDef GPIO_InitStruct = {
		.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14,
		.GPIO_Mode = GPIO_Mode_OUT,
		.GPIO_Speed = GPIO_Speed_50MHz,
		.GPIO_OType = GPIO_OType_PP,
		.GPIO_PuPd = GPIO_PuPd_NOPULL
	};

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

	GPIO_Init( GPIOG, &GPIO_InitStruct );
}

/* delayMsec is the period of flashing LED
 */
void flash_LED_task( unsigned int delayMsec )
{
	delayMsec >>= 2;

	while (1)
	{
		GPIO_SetBits( GPIOG, GPIO_Pin_13 );
		GPIO_ResetBits( GPIOG, GPIO_Pin_14 );

		sleep( delayMsec );

		GPIO_SetBits( GPIOG, GPIO_Pin_14 );
		GPIO_ResetBits( GPIOG, GPIO_Pin_13 );
		
		sleep( delayMsec );
	}
}
