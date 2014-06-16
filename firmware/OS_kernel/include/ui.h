#ifndef UI_H
#define UI_H

// define color for drawing beat cycle
#define GREEN 0
#define RED 1

// define beat count limit
#define BEATLIMIT 8

// define UI task status
#define TUNER_TASK 0;
#define METRONOME_TASK 1;

void ui_draw_beat(int color);

void ui_init();
void ui_start_tuner();
void ui_start_metronome();

#endif
