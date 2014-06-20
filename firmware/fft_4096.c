/* The code is referenced to 吉帥虎 ( Original
 * link is lost ), Ver 1.2, 2010-2-20
 */
#include <stdint.h>

#include "fft_4096.h"
#include "arm_math.h"

static struct cmplx fft_input[ DATA_LENGTH ];

/* Store the signal to real part of fft_input
 * and set the image part of fft_input to 0.
 */
void fft_input_init( int16_t *data )
{
	int i;

	for ( i = 0; i < DATA_LENGTH; ++i )
	{
		arm_q15_to_float( &data[i], &fft_input[i].real, 1 );
		fft_input[i].imag = 0.0f;
	}
}

void bit_reversal()
{
	int i, j = 0, k;
	struct cmplx tmp;

	for ( i = 0; i < DATA_LENGTH_MINUS_1; ++i )
	{
		if ( i < j )
		{
			tmp = fft_input[j];
			fft_input[j] = fft_input[i];
			fft_input[i] = tmp;
		}

		k = HALF_DATA_LENGTH;

		while ( k <= j )
		{
			j = j - k;
			k >>= 1;
		}

		j = j + k;
	}

}	// end of bit_reversal()

/* Multiply two complex number.
 * Return the result.
 */
struct cmplx EE( struct cmplx a, struct cmplx b )
{
	struct cmplx c;

	c.real = a.real * b.real - a.imag * b.imag;
	c.imag = a.real * b.imag + a.imag * b.real;

	return c;
}

/* The main process of FFT.
 * Using the butterfly algorithm.
 */
void butterfly()
{
	int le, lei, ip, m, f = DATA_LENGTH;
	int level, i, j;
	struct cmplx u, w, t;

	// Calculate the level of butterfly algorithm
	for ( level = 1; (f>>=1) != 1; ++level )
		;

	for ( m = 1; m <= level; ++m )
	{
		le = 2 << ( m - 1 );
		lei = le >> 1;

		u.real = 1.0;
		u.imag = 0.0;

		w.real = arm_cos_f32( PI / lei );
		w.imag = -arm_sin_f32( PI / lei );

		for ( j = 0; j <= lei - 1; ++j )
		{
			for ( i = j; i < DATA_LENGTH; i = i + le )
			{
				ip = i + lei;
				t = EE( fft_input[ip], u );
				fft_input[ip].real = fft_input[i].real - t.real;
				fft_input[ip].imag = fft_input[i].imag - t.imag;
				fft_input[i].real = fft_input[i].real + t.real;
				fft_input[i].imag = fft_input[i].imag + t.imag;
			}
			u = EE( u, w );
		}
	}

}	// end of bufferfly()

/* Do FFT for 4096 points of data.
 *
 * Return the frequency which has the max response.
 */
int fft_4096( int16_t *data )
{
	int i, maxIndex;
	float maxResponse = 0.0;

	fft_input_init( data );
	bit_reversal();
	butterfly();

	// Magnitude of complex number
	for ( i = 0; i < DATA_LENGTH; ++i )
	{
		arm_sqrt_f32( fft_input[i].real * fft_input[i].real +
				fft_input[i].imag * fft_input[i].imag,
				&fft_input[i].real );

		fft_input[i].real /= ( i == 0 ? DATA_LENGTH : DATA_LENGTH / 2 );
	}

	// Find the max response index
	// Only care about the first half of the fft output
	for ( i = 0; i < HALF_DATA_LENGTH; ++i )
	{
		if ( fft_input[i].real > maxResponse )
		{
			maxResponse = fft_input[i].real;
			maxIndex = i;
		}
	}

	return maxIndex;

}	// end of fft_4096()
