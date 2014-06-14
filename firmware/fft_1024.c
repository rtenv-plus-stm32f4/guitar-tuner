#include "fft_1024.h"

/* Using CMSIS libray for Cortex-CM4 */
#define ARM_MATH_CM4

#include "arm_math.h"
#include "arm_const_structs.h"

static float fft_output[ FFT_DATA_LENGTH / 2 ];

float fft_1024( float *signal, int sampleRate )
{
	int ifftFlag = 0;		// Forward FFT
	int doBitReverse = 1;	// Enable bit reversal of output
	int outSize = FFT_DATA_LENGTH / 2;
	uint32_t maxIndex;
	float maxResponse;

	/* Process the data through the CFFT/CIFFT module */
	arm_cfft_f32( &arm_cfft_sR_f32_len512, signal, ifftFlag, doBitReverse );
	/* Process the data through the Complex Magnitude Module for
	 * calculating the magnitude at each bin */
	arm_cmplx_mag_f32( signal, fft_output, outSize );
	/* Get the index of max response and value */
	arm_max_f32( fft_output, outSize, &maxResponse, &maxIndex );

	/* return the frequency which has highest response */
	return ((float)( sampleRate / 2 )) / outSize * (float)maxIndex;
}
