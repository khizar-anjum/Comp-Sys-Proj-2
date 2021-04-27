#include "TeamCreate.h"

void handleSignal(int sig) {
    printf("Signal %i handled by thread %i\n",sig,(int)pthread_self());
    signal(sig, SIG_DFL);
}

void blockSignal(int sig) {
    signal(sig, blockSignal);
}

void blockAllSignals() {
    int sigs[] = {SIGINT, SIGABRT, SIGILL, SIGCHLD, SIGSEGV, SIGFPE, SIGHUP, SIGTSTP};
    for(int i = 0; i < 8; i++) signal(sigs[i], blockSignal);
}

void unblockAllSignals() {
    int sigs[] = {SIGINT, SIGABRT, SIGILL, SIGCHLD, SIGSEGV, SIGFPE, SIGHUP, SIGTSTP};
    for(int i = 0; i < 8; i++) signal(sigs[i], SIG_DFL);
}

void initTeam(int* signals) {
    blockAllSignals();
    for(int i = 0; i < NUM_HANDLED; i++) signal(signals[i], handleSignal);
}

void* runTeam(void* tin) {
    ThreadInput* threadIn = (ThreadInput*)tin;

    initTeam(threadIn->signals);
    threadIn->target(threadIn->args, threadIn->num_args);
}

//usage:
//  signals -> signals to be handled by the thread
//  target -> function to be ran by the thread
//  num_args -> number of arguments in to be passed
//  ... -> arguments to be passed (in the form of pointers)
pthread_t createTeam(int* signals, ThreadFunc target, int num_args, ...) {
    va_list args; va_start(args, num_args);

    ThreadInput* threadIn = malloc(sizeof(ThreadInput));
    threadIn->target = target;
    threadIn->signals = signals;

    threadIn->num_args = num_args;
    threadIn->args = (num_args) ? calloc(num_args,sizeof(void*)):NULL;
    for(int i = 0; i < num_args; i++) {
        threadIn->args[i] = va_arg(args, void*);
    }
    va_end(args);

    pthread_t teamMember;
    if(target) pthread_create(&teamMember, 0, runTeam, threadIn);
    else teamMember = 0;
    return teamMember;
}