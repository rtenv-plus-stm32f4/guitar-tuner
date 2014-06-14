#include "tuner.h"
#include "fft_1024.h"
#include "buzz_C3.h"

#define SAMPLE_RATE	44100

void tuner_task()
{
	extern float test_signal[ FFT_DATA_LENGTH ];
	float responseFreq;

	responseFreq = fft_1024( test_signal, SAMPLE_RATE );
	responseFreq += responseFreq;

	while(1);
}
