#ifndef _QUICK_SORT_H
#define _QUICK_SORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    int* nums;
    int* sorted;
    int len;
    int pos;
    int numThreads;
} SortThreadInput;

typedef struct {
    int* nums;
    int len;
    int numThreads;
    int trialNo;
} Inputs;

int getPosition(int* nums, int len, int pos);
void sortThreadHelper(int* nums, int* sorted, int len, int pos, int numThreads);
void* sortThread(void* vi);
void packThreadInput(SortThreadInput* si, int* nums, int* sorted, int len, int pos, int numThreads);
void sortHelper(int* nums, int len, int numThreads);
Inputs* packInputs(int* nums, int len, int numThreads, int trialNo);
void* sort(void* vinputs);

#ifdef __cplusplus
}
#endif

#endif