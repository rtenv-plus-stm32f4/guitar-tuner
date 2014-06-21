#include <stdint.h>
#include "syscall.h"
#include "semaphore.h"
#include <stdint.h>

#include "syscall.h"
#include "tuner.h"
#include "fft_4096.h"
#include "buzz_C3.h"

#define SAMPLE_RATE	4096

extern int responseFreq;

#include "main.h"

extern semaphore_t tuner_sem;
extern semaphore_t metronome_sem;

extern int mode;

void tuner_task()
{
        int num = 0;
	extern int16_t int_sample[ DATA_LENGTH ];
        extern uint16_t base;
        extern uint32_t voltage;

        while(1){
            sleep(1);
            voltage = base * 3000 / 0xFFFF;
            int_sample[num++] = voltage;

            if(num % 512 == 0)
                responseFreq = fft_4096( int_sample );
            if(num == DATA_LENGTH)
                num = 0;
        }
}
