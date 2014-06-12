#include "syscall.h"
#include "kernel.h"
#include "path.h"
#include "romfs.h"
#include "ui.h"

#include "metronome.h"

#define MODE METRONOME

void UI_task()
{
    ui_init();
}

void first()
{
	if (!fork()) setpriority(0, 0), pathserver();
	if (!fork()) setpriority(0, 0), UI_task();

#if MODE == METRONOME
	if (!fork()) setpriority(0, 1), metronome_task();
#elif MODE == TUNER
	if (!fork()) setpriority(0, 1), tuner_task();
#endif

	setpriority(0, PRIORITY_LIMIT);

	mount("/dev/rom0", "/", ROMFS_TYPE, 0);

	while(1);
}

int main()
{
	/* Hardware Initialization */
	buzzer_init();

	/* Start to schedule */
	rtenv_start_scheduler(first);
	
	return 0;
}
