#include "tuner.h"
#include "fft_4096.h"
#include "buzz_C3.h"

#define SAMPLE_RATE	4096

int responseFreq;

void tuner_task()
{
	extern float test_signal[ DATA_LENGTH ];

	responseFreq = fft_4096( test_signal );
	responseFreq += responseFreq;

	while(1);
}
