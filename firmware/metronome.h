#ifndef __METRONOME
#define __METRONOME

#define DEFAULT_METRONOME_BPM 120
#define DEFAULT_BEAT_CNT 4

extern int metronome_bpm;
extern int metronome_beat_count;

void buzzer_init();
void beep(int frequency);

void metronome_task();

#endif
