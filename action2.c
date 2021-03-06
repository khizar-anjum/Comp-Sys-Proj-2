#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "Sort.h"

#define NUMS_LEN 1000000
#define TRIAL_1 10
#define TRIAL_2 100
#define TRIAL_3 1000

void runTrial(int* nums, int len, int numThreads, int trialNo) {
    pthread_t threads[4];
    Inputs** inputs = (Inputs**)calloc(4,sizeof(Inputs*));
    int sublen = len / 4;

    for(int i = 0; i < 4; i++) {
        inputs[i] = packInputs(nums,sublen,numThreads,trialNo);
        pthread_create(&threads[i], 0, sort, inputs[i]);
        nums += sublen;
    }

    for(int i = 0; i < 4; i++) {
        pthread_join(threads[i],NULL);
        free(inputs[i]);
    } free(inputs);
}

int main() {
    srand((unsigned)time(NULL));

    int* nums = (int*)calloc(NUMS_LEN,sizeof(int));
    for(int i = 0; i < NUMS_LEN; i ++) nums[i] = rand();

    printf("\nRunning Trial 1 (%i threads)\n",TRIAL_1);
    runTrial(nums, NUMS_LEN, TRIAL_1, 1);

    printf("\nRunning Trial 2 (%i threads)\n",TRIAL_2);
    runTrial(nums, NUMS_LEN, TRIAL_2, 2);

    printf("\nRunning Trial 3 (%i threads)\n",TRIAL_3);
    runTrial(nums, NUMS_LEN, TRIAL_3, 3);

    free(nums);

    printf("Main thread is done!\n");
    return 1;
}