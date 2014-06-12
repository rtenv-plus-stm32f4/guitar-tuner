#include "syscall.h"
#include "metronome.h"

/* TODO: Finish this with buzzer operating implemantation */
#define FIRST_BEEP_FREQ  0 
#define NORMAL_BEEP_FREQ 1

int metronome_bpm = DEFAULT_METRONOME_BPM;
int beat_count = DEFAULT_BEAT_CNT;

void buzzer_init()
{
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
		if(current_beat == 0 && beat_count != 1) {
			beep(FIRST_BEEP_FREQ);
		} else {
			beep(NORMAL_BEEP_FREQ);
		}

		current_beat = (current_beat + 1) % beat_count;
		
		/* Delay for a cycle */
		sleep(cycle_time);
	}
}
