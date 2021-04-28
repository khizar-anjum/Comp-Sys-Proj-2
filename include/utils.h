#ifndef UTILS__H
#define UTILS__H

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

struct thread_params
{
	int team_no;
};

void simple_sighandler(int signum, siginfo_t *info, void *ptr);
void *pthread_func(void* vargp);


#endif