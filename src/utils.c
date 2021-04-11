#include "utils.h"

void simple_sighandler(int signum, siginfo_t *info, void *ptr)	{
	printf("Hi I am process %d and I am handling signal %d\n", getpid(), signum);
	printf("Signal originates from process %lu\n", (unsigned long)info->si_pid);
}
