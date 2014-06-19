#include "syscall.h"
#include "metronome.h"
#include "stm32f4xx.h"

/* TODO: Finish this with buzzer operating implemantation */
#define FIRST_BEEP_FREQ  0 
#define NORMAL_BEEP_FREQ 1

int metronome_bpm = DEFAULT_METRONOME_BPM;
int metronome_beat_count = DEFAULT_BEAT_CNT;

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

uint16_t CCR1_Val = 350;
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
    /* TIM2 enable counter */
    TIM_Cmd(TIM2, ENABLE);
}

void buzzer_init()
{
    TIM_config();

  while(1){}
}

void beep(int frequency)
{
}

void metronome_task()
{
	int current_beat = 0;
	int cycle_time = 0;

	while(1) {
		/* Calculate the beat cycle time */
		cycle_time = (60 * 1000) / metronome_bpm; /* Cycle time = 1 minute / BPM */

		/* First beat and beat count is setting as 0 or bigger then 1 */
		if(current_beat == 0 && metronome_beat_count != 1) {
			beep(FIRST_BEEP_FREQ);
		} else {
			beep(NORMAL_BEEP_FREQ);
		}

		current_beat = (current_beat + 1) % metronome_beat_count;
		
		/* Delay for a cycle */
		sleep(cycle_time);
	}
}
