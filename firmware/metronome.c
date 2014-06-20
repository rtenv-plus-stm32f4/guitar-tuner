#include "stm32f4xx.h"
#include "syscall.h"
#include "semaphore.h"
#include "metronome.h"
#include "ui.h"
#include "main.h"

#define STOP    0
#define RUNNING 1

extern semaphore_t tuner_sem;
extern semaphore_t metronome_sem;

extern int mode;

int metronome_bpm = DEFAULT_METRONOME_BPM;
int metronome_beat_count = DEFAULT_BEAT_CNT;
int cycle_time = 0;
int current_beat = 0;
int beat_draw;
int metro_status = STOP;

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

uint16_t PrescalerValue = 0;

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
    }
}

void TIM_config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* TIM4 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    /* GPIOD clock enable */
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
      
    /* GPIOD Configuration: TIM4 CH1 (PD12), TIM4 CH2 (PD13), TIM4 CH3 (PD14) and TIM4 CH4 (PD15) */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOD, &GPIO_InitStructure); 

    NVIC_InitTypeDef NVIC_InitStructure;
    /* Enable the TIM2 gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
 
    /* TIM2 clock enable */
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1; // 1 MHz down to 1 KHz (1 ms)
    TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1; // 24 MHz Clock down to 1 MHz (adjust per your clock)
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    /* TIM IT enable */
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, DISABLE);
}

void buzzer_init()
{
    TIM_config();
}

void beep(int frequency)
{

    switch(frequency){
        case NORMAL_BEEP_FREQ:
            TIM_TimeBaseStructure.TIM_Period = 1000 - 1; // 1 MHz down to 1 KHz (1 ms)
            break;
        case FIRST_BEEP_FREQ:
            TIM_TimeBaseStructure.TIM_Period = 500 - 1; // 1 MHz down to 1 KHz (1 ms)
            break;
    }

    //TIM2 reinit
    TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1; // 24 MHz Clock down to 1 MHz (adjust per your clock)
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /* TIM2 enable counter */
    TIM_Cmd(TIM2, ENABLE);

    sleep(20);

    TIM_Cmd(TIM2, DISABLE);
}

void metronome_task()
{
	while(1) {
		while(mode == TUNER_MODE);
		while(!(metro_status));

		/* Calculate the beat cycle time */
		cycle_time = (60 * 1000) / metronome_bpm / 16; /* Cycle time = 1 minute / BPM */

		/* First beat and beat count is setting as 0 or bigger then 1 */
		if(current_beat == 0 && metronome_beat_count != 1) {
			beep(FIRST_BEEP_FREQ);
			ui_draw_beat(RED, cycle_time * 9 / 10);
		} else {
			beep(NORMAL_BEEP_FREQ);
			ui_draw_beat(GREEN, cycle_time * 9 / 10);
		}

		beat_draw = 1;
		current_beat = (current_beat + 1) % metronome_beat_count;
		
		/* Delay for a cycle */
		sleep(cycle_time / 10);
	}
}

