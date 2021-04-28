#include "Action2.h"

void runTrial(int* nums, int len, int numThreads) {
    blockAllSignals();

    int signalTeams[4][3];
    signalTeams[0][0] = SIGINT; signalTeams[0][1] = SIGABRT; signalTeams[0][2] = SIGCHLD;
    signalTeams[1][0] = SIGINT; signalTeams[1][1] = SIGILL; signalTeams[1][2] = SIGCHLD;
    signalTeams[2][0] = SIGINT; signalTeams[2][1] = SIGCHLD; signalTeams[2][2] = SIGFPE;
    signalTeams[3][0] = SIGINT; signalTeams[3][1] = SIGFPE; signalTeams[3][2] = SIGHUP;

    pthread_t threads[4];
    int sublen = len / 4;
    for(int i = 0; i < 4; i++) {
        threads[i] = createTeam(signalTeams[i], sort, 3, nums, &sublen, &numThreads);
        nums += sublen;
    }

    for(int i = 0; i < 4; i++) {
        pthread_join(threads[i],NULL);
    }

    unblockAllSignals();
}

int main() {
    srand((unsigned)time(NULL));

    int* nums = calloc(NUMS_LEN,sizeof(int));
    for(int i = 0; i < NUMS_LEN; i ++) nums[i] = rand();

    printf("\nRunning Trial 1 (%i threads):\n",TRIAL_1);
    runTrial(nums, NUMS_LEN, TRIAL_1);

    printf("\nRunning Trial 2 (%i threads):\n",TRIAL_2);
    runTrial(nums, NUMS_LEN, TRIAL_2);

    printf("\nRunning Trial 3 (%i threads):\n",TRIAL_3);
    runTrial(nums, NUMS_LEN, TRIAL_3);

    free(nums);

    printf("Main thread is done!\n");
    return 1;
}