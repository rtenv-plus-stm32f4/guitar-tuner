#include "syscall.h"
#include "kernel.h"
#include "semaphore.h"
#include "path.h"
#include "romfs.h"

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"

#include "ui.h"
#include "metronome.h"
#include "tuner.h"
#include "main.h"

semaphore_t tuner_sem = 1; //Default mode
semaphore_t metronome_sem = 0;

int mode = TUNER_MODE;

void UI_task()
{
}

void button_task()
{
	while(1) {
		/* User press down the button */
		while(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0));
		/* User release the button */
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0));

		mode = (mode + 1) % 2;
	}
}

void button_init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct = {
		.GPIO_Pin = GPIO_Pin_0,
		.GPIO_Mode = GPIO_Mode_IN,
		.GPIO_Speed = GPIO_Speed_100MHz
	};

	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void first()
{
	if (!fork()) setpriority(0, 0), pathserver();
	if (!fork()) setpriority(0, 1), UI_task();

	if (!fork()) setpriority(0, 1), metronome_task();
	if (!fork()) setpriority(0, 1), tuner_task();
	if (!fork()) setpriority(0, 1), button_task();

	setpriority(0, PRIORITY_LIMIT);

	mount("/dev/rom0", "/", ROMFS_TYPE, 0);

	while(1);
}

int main()
{
	/* Hardware Initialization */
	buzzer_init();
	button_init();
	ui_init();

	/* Start to schedule */
	rtenv_start_scheduler(first);
	
	return 0;
}
