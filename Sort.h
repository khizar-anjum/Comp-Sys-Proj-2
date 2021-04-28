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

int getPosition(int* nums, int len, int pos);
void sortThreadHelper(int* nums, int* sorted, int len, int pos, int numThreads);
void* sortThread(void* vi);
void packThreadInput(SortThreadInput* si, int* nums, int* sorted, int len, int pos, int numThreads);
void sortHelper(int* nums, int len, int numThreads);
void* sort(void** args);

#ifdef __cplusplus
}
#endif

#endif