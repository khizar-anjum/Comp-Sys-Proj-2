#include "Action3.h"
pthread_mutex_t lock;
volatile int block;
pthread_t threads[4];

void* action3Func(void* vinputs) {
    sort(vinputs);
    pthread_mutex_lock(&lock);
    printf("Trial %i was the fastest\n",pthread_self());
    for(int i = 0; i < 4; i++) {
        if(threads[i] != pthread_self()) {
            pthread_kill(threads[i],TO_SEND);
            pthread_kill(threads[i],TO_SEND);
        }
    }
    pthread_mutex_unlock(&lock);
    pthread_kill(pthread_self(),TO_SEND);
    pthread_kill(pthread_self(),TO_SEND);
    return NULL;
}

void runTrial(int* nums, int len, int numThread, int trialNo) {
    // initilize
	int success;
	sigset_t to_block_main;
	
	// Lets prepare signal handling for main
	// Block all signals for main until the threads are created
	printf("Main: Blocking all signals ...\n");
	sigfillset(&to_block_main);
	sigprocmask(SIG_BLOCK, &to_block_main, NULL);

    Inputs** inputs = (Inputs**)calloc(4,sizeof(Inputs*));
    int sublen = len / 4;

    for(int i = 0; i < 4; i++) {
        inputs[i] = packInputs(nums,sublen,numThread,trialNo);
        pthread_create(&threads[i], 0, action3Func, inputs[i]);
        nums += sublen;
    }

    printf("Main: Creating Threads ...\n");
	printf("Main: Unblocking ... Threads will handle it from now on ...\n");
	sigprocmask(SIG_UNBLOCK, &to_block_main, NULL);

    for(int i = 0; i < 4; i++) {
        pthread_join(threads[i],NULL);
        free(inputs[i]);
    } free(inputs);
}

int main() {
    pthread_mutex_init(&lock, NULL); block = 0;
    srand((unsigned)time(NULL));

    int* nums = (int*)calloc(NUMS_LEN,sizeof(int));
    for(int i = 0; i < NUMS_LEN; i ++) nums[i] = rand();

    printf("\nRunning Trial 2 (%i threads):\n",TRIAL_2);
    runTrial(nums, NUMS_LEN, TRIAL_2, 2);
    free(nums);

    printf("Main thread is done!\n");
    return 1;
}