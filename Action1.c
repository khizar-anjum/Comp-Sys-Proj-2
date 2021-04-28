#include "Action1.h"

void* action1Func(void** args) {
    printf("Thread %i in action fuction\n",(int)pthread_self());
    sleep(SLEEP_TIME);
    printf("Thread %i finished action fuction\n",(int)pthread_self());
    return NULL;
}

int main() {
    blockAllSignals();

    int signalTeams[4][3];
    signalTeams[0][0] = SIGINT; signalTeams[0][1] = SIGABRT; signalTeams[0][2] = SIGCHLD;
    signalTeams[1][0] = SIGINT; signalTeams[1][1] = SIGILL; signalTeams[1][2] = SIGCHLD;
    signalTeams[2][0] = SIGINT; signalTeams[2][1] = SIGCHLD; signalTeams[2][2] = SIGFPE;
    signalTeams[3][0] = SIGINT; signalTeams[3][1] = SIGFPE; signalTeams[3][2] = SIGHUP;

    pthread_t threads[4];
    for(int i = 0; i < 4; i++) {
        threads[i] = createTeam(signalTeams[i], action1Func, 0);
    }

    pthread_kill(threads[0],SIGINT);
    pthread_kill(threads[2],SIGFPE);
    pthread_kill(threads[3],SIGTSTP);

    for(int i = 0; i < 4; i++) {
        pthread_join(threads[i],NULL);
    }

    unblockAllSignals();

    printf("Main thread is done!\n");
    return 1;
}