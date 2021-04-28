#include "Action3.h"
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_t threads[4];

void* action3Func(void* vinputs) {
    sleep(1);
    sort(vinputs);
    pthread_mutex_lock(&lock);
    printf("Thread %i was the fastest\n",pthread_self());
    for(int i = 0; i < 4; i++) {
        if(threads[i] != pthread_self()) {
            pthread_cancel(threads[i]);
        }
    }
    for(int i = 0; i < 4; i++) {
        if(threads[i] != pthread_self()) {
            pthread_join(threads[i],NULL);
        }
    }
    pthread_mutex_unlock(&lock);
    pthread_cancel(pthread_self());
    return NULL;
}

void runTrial(int* nums, int len, int numThread, int trialNo) {

    Inputs** inputs = (Inputs**)calloc(4,sizeof(Inputs*));
    int sublen = len / 4;

    for(int i = 0; i < 4; i++) {
        inputs[i] = packInputs(nums,sublen,numThread,trialNo);
        pthread_create(&threads[i], 0, action3Func, inputs[i]);
        nums += sublen;
    }

    sleep(10);

    for(int i = 0; i < 4; i++) {
        free(inputs[i]);
    } free(inputs);
}

int main() {
    srand((unsigned)time(NULL));

    int* nums = (int*)calloc(NUMS_LEN,sizeof(int));
    for(int i = 0; i < NUMS_LEN; i ++) nums[i] = rand();

    printf("\nRunning Trial 2 (%i threads):\n",TRIAL_2);
    runTrial(nums, NUMS_LEN, TRIAL_2, 2);
    free(nums);

    printf("Main thread is done!\n");
    return 1;
}