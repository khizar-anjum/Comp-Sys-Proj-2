#ifndef _TEAM_CREATE_H
#define _TEAM_CREATE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <signal.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* (*ThreadFunc) (void** args, int num_args);
typedef struct {
    ThreadFunc target;
    void** args;
    int num_args;

    int* signals;
} ThreadInput;

#define NUM_HANDLED 3

void handleSignal(int sig);
void blockSignal(int sig);
void blockAllSignals();
void unblockAllSignals();
void initTeam(int* signals);
void* runTeam(void* tin);
pthread_t createTeam(int* signals, ThreadFunc target, int num_args, ...);

#ifdef __cplusplus
}
#endif

#endif