#ifndef _ACTION_1_H
#define _ACTION_1_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "TeamCreate.h"

void* action1Func(void** args, int num_args);
int main();

#define SLEEP_TIME 2

#endif