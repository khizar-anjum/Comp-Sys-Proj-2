#ifndef _ACTION_2_H
#define _ACTION_2_H

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "Sort.h"

#define NUMS_LEN 1000000
#define TRIAL_1 10
#define TRIAL_2 100
#define TRIAL_3 1000

void runTrial(int* nums, int len, int numThreads, int trialNo);

#endif