#include "utils.h"

void simple_sighandler(int signum, siginfo_t *info, void *ptr)	{
	printf("Hi I am process %d and I am handling signal %d\n", getpid(), signum);
	printf("Signal originates from process %lu\n", (unsigned long)info->si_pid);
}

void *pthread_func(void* vargp){
	struct thread_params* tps = (struct thread_params*) vargp;
	printf("Hello this is thread %d\n", tps->team_no);

	struct sigaction t_params;
	int success;
	t_params.sa_sigaction = simple_sighandler;
	t_params.sa_flags = SA_SIGINFO; // | SA_RESTART

	// check which threads to deploy (ASSUMING 4 TEAMS)
	switch(tps->team_no){
		case 0:
			success = sigaction(SIGINT, &t_params, NULL);
			break;
		case 1:
			success = sigaction(SIGABRT, &t_params, NULL);
			break;
		case 2:
			success = sigaction(SIGILL, &t_params, NULL);
			break;
		case 3:
			success = sigaction(SIGCHLD, &t_params, NULL);
			break;
		default:
			printf("Invalid Team!\n");
			pthread_exit(0);
	}
	

}
