#include "syscall.h"
#include "semaphore.h"
#include "tuner.h"

#include "main.h"

extern semaphore_t tuner_sem;
extern semaphore_t metronome_sem;

extern int mode;

void tuner_task()
{
	wait(&tuner_sem);

	while(1) {
		if(mode == METRONOME) {
			signal(&metronome_sem);
			wait(&tuner_sem);
		}
	}
}
