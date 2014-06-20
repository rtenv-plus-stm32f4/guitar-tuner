#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ioe.h"
#include "syscall.h"
#include <string.h>
#include "metronome.h"
#include "ui.h"
#include "main.h"

static uint8_t sound[8][2] = {"A", "B", "C", "D", "E", "F", "G", "H"};
static uint8_t hz_str[3] = "Hz";
static uint8_t frequency_str[4];
static uint8_t bpm_text_str[4] = "BPM";
static uint8_t beat_text_str[5] = "BEAT";
static uint8_t bpm_str[4];
static uint8_t beat_count_str[2];

extern int metronome_bpm;
extern int metronome_beat_count;

extern int mode;

int show_layer = 0;
int hidden_layer = 1;

static void TP_Config(void);

struct layer_buffer{
    LTDC_Layer_TypeDef *LTDC_Layer;
    int LCD_Layer;
    void *buffer;
}layer_buffers[] = {
    {LTDC_Layer1,LCD_BACKGROUND_LAYER, (void*)LCD_FRAME_BUFFER},
    {LTDC_Layer2,LCD_FOREGROUND_LAYER,(void*) LCD_FRAME_BUFFER + BUFFER_OFFSET}
};

static uint8_t* itoa(int value, uint8_t* result, int base)
{
	if (base < 2 || base > 36) {
		*result = '\0';
		return result;
	}
	uint8_t *ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while (value);

	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while (ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr-- = *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}

void ui_swap_layer()
{
    LTDC_LayerAlpha(layer_buffers[hidden_layer].LTDC_Layer, 0xff);
    LTDC_LayerAlpha(layer_buffers[show_layer].LTDC_Layer, 0x00);
    LCD_SetLayer(layer_buffers[show_layer].LCD_Layer);
    LTDC_ReloadConfig(LTDC_IMReload);
    LCD_Clear(LCD_COLOR_WHITE);

    show_layer = !show_layer;
    hidden_layer = !hidden_layer;

}

void ui_draw_beat(int color)
{
    if(mode != METRONOME_MODE){
        return;
    }

    switch(color){
        case GREEN:
            LCD_SetColors(LCD_COLOR_GREEN, LCD_COLOR_GREEN);
            break;
        case RED:
            LCD_SetColors(LCD_COLOR_RED, LCD_COLOR_RED);
            break;
    }

    LCD_DrawFullCircle(200, 40, 10);
    ui_start_metronome();


    sleep(50);
    
    // clear the circle
    LCD_SetColors(LCD_COLOR_WHITE, LCD_COLOR_WHITE);
    LCD_DrawFullCircle(200, 40, 10);
    ui_start_metronome();
}

void ui_draw_note()
{
    LCD_SetColors(LCD_COLOR_BLACK , LCD_COLOR_BLACK);
    LCD_DrawFullEllipse(160, 260, 50, 35);
    
    LCD_DrawLine(160 + 50, 80, 180, LCD_DIR_VERTICAL);
    LCD_DrawLine(160 + 50 -1, 80, 180, LCD_DIR_VERTICAL);
    LCD_DrawLine(160 + 50 -2, 80, 180, LCD_DIR_VERTICAL);
    LCD_DrawLine(160 + 50 -3, 80, 180, LCD_DIR_VERTICAL);
    LCD_DrawLine(160 + 50 -4, 80, 180, LCD_DIR_VERTICAL);
}

void ui_draw_button()
{
    LCD_SetColors(LCD_COLOR_BLACK , LCD_COLOR_BLACK);
    LCD_DrawFullRect(70     , 55     , 40, 40);
    LCD_DrawFullRect(70 + 60, 55     , 40, 40);
    LCD_DrawFullRect(70     , 55 + 75, 40, 40);
    LCD_DrawFullRect(70 + 60, 55 + 75, 40, 40);

    LCD_SetColors(LCD_COLOR_YELLOW , LCD_COLOR_YELLOW);
    //BPM minus
    LCD_DrawFullRect(70 + 5 , 55 + 15, 30, 10);

    //BPM plus
    LCD_DrawFullRect(70 + 65, 55 + 15, 30, 10);
    LCD_DrawFullRect(70 + 75, 55 + 5 , 10, 30);

    //BEAT minus
    LCD_DrawFullRect(70 + 5 , 55 + 90, 30, 10);

    //BEAT plus
    LCD_DrawFullRect(70 + 65, 55 + 90, 30, 10);
    LCD_DrawFullRect(70 + 75, 55 + 80, 10, 30);

}

void TP_Config()
{
  IOE_Config();
}

void ui_touch_detect()
{
    static TP_STATE* TP_State; 

    TP_State = IOE_TP_GetState();

    if(TP_State->TouchDetected){
        //bpm mius
        if(TP_State->X > 70 && TP_State->X < 70+40 && TP_State->Y > 55 && TP_State->Y < 55 + 40){
            metronome_bpm--;
            if(metronome_bpm < 40){
                metronome_bpm = 208;
            }
        }
        //bpm plus
        if(TP_State->X > 70+60 && TP_State->X < 70+40+60 && TP_State->Y > 55 && TP_State->Y < 55 + 40){
            metronome_bpm++;
            if(metronome_bpm > 208){
                metronome_bpm = 40;
            }
        }
        //beat minus
        if(TP_State->X > 70 && TP_State->X < 70+40 && TP_State->Y > 55+75 && TP_State->Y < 55+40+75){
            metronome_beat_count--;
            if(metronome_beat_count < 0){
                metronome_beat_count = 7;
            }
        }
        //beat plus
        if(TP_State->X > 70+60 && TP_State->X < 70+40+60 && TP_State->Y > 55+75 && TP_State->Y < 55+40+75){
            metronome_beat_count++;
            if(metronome_beat_count > 7){
                metronome_beat_count = 0;
            }
        }
    }
}

void ui_start_tuner()
{

    int hz = 332, i = 0;

    itoa(hz, frequency_str, 10);
    strcat((char *)frequency_str, (char *)hz_str);

    LCD_SetColors(LCD_COLOR_MAGENTA , LCD_COLOR_WHITE);
    
    LCD_DrawLine(30, 200, 180, LCD_DIR_HORIZONTAL);
    LCD_DrawLine(30, 180, 40, LCD_DIR_VERTICAL);
    //LCD_DrawLine(230, 180, 40, LCD_DIR_VERTICAL);

    //draw scale
    for(i = 1; i <= 6; i++){
        LCD_DrawLine(30 + 30*i, 180, 40, LCD_DIR_VERTICAL);
    }

    LCD_DisplayStringLine(LCD_LINE_2, sound[3]);
    LCD_DisplayStringLine(LCD_LINE_3, frequency_str);

    ui_swap_layer();

}

void ui_start_metronome()
{

    itoa(metronome_bpm, bpm_str, 10);
    
    metronome_beat_count = metronome_beat_count % BEATLIMIT;
    
    itoa(metronome_beat_count, beat_count_str, 10);
    
    ui_draw_note();

    ui_draw_button();

    LCD_SetColors(LCD_COLOR_MAGENTA , LCD_COLOR_WHITE);
        
    LCD_DisplayStringLine(LCD_LINE_2, bpm_text_str);
    LCD_DisplayStringLine(LCD_LINE_3, bpm_str);
        
    LCD_DisplayStringLine(LCD_LINE_5, beat_text_str);
    LCD_DisplayStringLine(LCD_LINE_6, beat_count_str);

    ui_touch_detect();

    ui_swap_layer();


}

void ui_init()
{
    LCD_Init();

    LCD_LayerInit();

    LTDC_Cmd(ENABLE);

    LCD_DisplayOn();
}

void ui_task()
{
    LCD_SetLayer(layer_buffers[hidden_layer].LCD_Layer);
    ui_swap_layer();

    TP_Config();

    mode = METRONOME_MODE;

    while(1){
        if(mode == TUNER_MODE){
            ui_start_tuner();
        }
        if(mode == METRONOME_MODE){
            ui_start_metronome();
        }
        sleep(100);
    }
    
}
