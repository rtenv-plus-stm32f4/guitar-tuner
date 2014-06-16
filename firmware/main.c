#include "syscall.h"
#include "kernel.h"
#include "semaphore.h"
#include "path.h"
#include "romfs.h"

#include "ui.h"
#include "metronome.h"
#include "tuner.h"

#define TUNER_MODE 0
#define METRONOME  1

semaphore_t tuner_sem = 1; //Default mode
semaphore_t metronome_sem = 0;

int mode = TUNER_MODE;

void UI_task()
{
    ui_init();
}

void first()
{
	if (!fork()) setpriority(0, 0), pathserver();
	if (!fork()) setpriority(0, 0), UI_task();

	if (!fork()) setpriority(0, 1), metronome_task();
	if (!fork()) setpriority(0, 1), tuner_task();

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
