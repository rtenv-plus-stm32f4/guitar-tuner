#ifndef KERNEL_H
#define KERNEL_H

#include "task.h"
#include "file.h"
#include "event-monitor.h"
#include <stddef.h>

void itoa(int n, char *dst, int base);
void rtenv_start_scheduler();

#endif
