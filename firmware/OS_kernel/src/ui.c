#include "stm32f429i_discovery_lcd.h"
#include "syscall.h"
#include <string.h>

static uint8_t sound[8][2] = {"A", "B", "C", "D", "E", "F", "G", "H"};
static uint8_t hz_str[3] = "Hz";
static uint8_t frequency_str[10];


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

void ui_bfclear()
{
	memset((void *) LCD_FRAME_BUFFER + BUFFER_OFFSET, 0xff, BUFFER_OFFSET);
	//memset((void *) LCD_FRAME_BUFFER, 0xff, BUFFER_OFFSET);
}


void ui_start_tuner()
{
    int hz = 332;

    itoa(hz, frequency_str, 10);
    strcat((char *)frequency_str, (char *)hz_str);

    LCD_SetColors(LCD_COLOR_MAGENTA , LCD_COLOR_WHITE);

    while(1){

        ui_bfclear();
    
        LCD_DisplayStringLine(LCD_LINE_3, sound[3]);
        LCD_DisplayStringLine(LCD_LINE_5, frequency_str);

        LCD_DrawLine(20, 200, 200, LCD_DIR_HORIZONTAL);
        LCD_DrawLine(20, 180, 40, LCD_DIR_VERTICAL);
        LCD_DrawLine(220, 180, 40, LCD_DIR_VERTICAL);

        sleep(1220);
    }
}

void ui_init()
{

    LCD_Init();

    LCD_LayerInit();

    LTDC_Cmd(ENABLE);

    LCD_DisplayOn();

    LCD_SetLayer(LCD_FOREGROUND_LAYER);

    ui_start_tuner();
}
