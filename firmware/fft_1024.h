#ifndef FFT_1024
#define FFT_1024

#define FFT_DATA_LENGTH		1024

/* Do FFT using ARM library with 1024 points of data.
 * - signal: [in,out] An array contain 1024 points of data
 *   sampleRate: [in] The sample rate of signal
 *
 * - Return: The frequency that has the maxium response */
float fft_1024( float *signal, int sampleRate );

#endif /* FFT_1024 */
