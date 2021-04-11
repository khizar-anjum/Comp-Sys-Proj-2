#ifndef UTILS__H
#define UTILS__H

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void simple_sighandler(int signum, siginfo_t *info, void *ptr);


#endif