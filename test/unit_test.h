#ifndef UNIT_TEST_H
#define UNIT_TEST_H

#include "kernel.h"

extern char cmd[HISTORY_COUNT][CMDBUF_SIZE];
extern int cur_his;

extern evar_entry env_var[MAX_ENVCOUNT];
extern int env_count;

void unit_test();

#endif
