#include "syscall.h"
#include "semaphore.h"
#include "tuner.h"
#include "fft_4096.h"
#include "buzz_C3.h"

#define SAMPLE_RATE	4096

int responseFreq;

#include "main.h"

extern semaphore_t tuner_sem;
extern semaphore_t metronome_sem;

extern int mode;

void tuner_task()
{
	//wait(&tuner_sem);

	extern float test_signal[ DATA_LENGTH ];

	responseFreq = fft_4096( test_signal );
	responseFreq += responseFreq;

	while(1) {
		while(mode == METRONOME_MODE);

		if(mode == METRONOME_MODE) {
			//signal(&metronome_sem);
			//wait(&tuner_sem);
		}
	}
}
