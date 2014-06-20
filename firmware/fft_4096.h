#ifndef FFT_4096
#define FFT_4096

#include <stdint.h>

#define DATA_LENGTH         4096
#define DATA_LENGTH_MINUS_1 4095
#define HALF_DATA_LENGTH    2048

#define PI 3.1415926535897932384626433832795028841971

/* The data structure of complex number */
struct cmplx {
	float real, imag;
};

int fft_4096( int16_t *data );

#endif /* FFT_4096 */
