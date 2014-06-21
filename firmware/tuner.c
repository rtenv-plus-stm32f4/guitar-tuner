#include <stdint.h>
#include "syscall.h"
#include "semaphore.h"
#include <stdint.h>

#include "syscall.h"
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
	extern int16_t test_signal[ DATA_LENGTH ];

	responseFreq = fft_4096( test_signal );
	responseFreq += responseFreq;

	while(1)
	{
		SLEEP(1);
	}
}
