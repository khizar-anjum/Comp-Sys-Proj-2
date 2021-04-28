#include "Sort.h"

int getPosition(int* nums, int len, int pos) {
    int spos = 0;
    int num = nums[pos];
    for(int i = 0; i < len; i++) {
        if((nums[i] < num) || (nums[i] == num && i < pos)) spos++;
    }
    return spos;
}

void sortThreadHelper(int* nums, int* sorted, int len, int pos, int numThreads) {
    for(int i = pos; i < len; i += numThreads) {
        int spos = getPosition(nums,len,i);
        sorted[spos] = nums[i];
    }
}

void* sortThread(void* vi) {
    SortThreadInput* si = (SortThreadInput*)vi;
    sortThreadHelper(si->nums, si->sorted, si->len, si->pos, si->numThreads);
}

void packThreadInput(SortThreadInput* si, int* nums, int* sorted, int len, int pos, int numThreads) {
    si->nums = nums;
    si->sorted = sorted;
    si->len = len;
    si->pos = pos;
    si->numThreads = numThreads;
}

void sortHelper(int* nums, int len, int numThreads) {
    pthread_t threads[numThreads];
    SortThreadInput** si = calloc(numThreads,sizeof(SortThreadInput*));
    int sorted[len];

    for(int i = 0; i < numThreads; i++) {
        si[i] = malloc(sizeof(SortThreadInput));
        packThreadInput(si[i], nums, sorted, len, i, numThreads);
        pthread_create(&threads[i], 0, sortThread, si[i]);
    }

    for(int i = 0; i < numThreads; i++) {
        pthread_join(threads[i],NULL);
        free(si[i]);
    }
    free(si);

    for(int i = 0; i < len; i++) nums[i] = sorted[i];
}

void* sort(void** args) {
    printf("Thread %i staring sort\n", (int)pthread_self());
    time_t start = time(NULL);
    sortHelper(args[0], *(int*)args[1], *(int*)args[2]);
    printf("Thread %i took %i seconds to sort\n", (int)pthread_self(), (int)(time(NULL)-start));
}