#include "utils.h"

#define NUM_TEAMS 4
#define THREADS_PER_TEAM 1


int main(int argc, char* argv[]){

	// initilize
	int success;
	sigset_t to_block_main;
	pthread_t thread_ids[NUM_TEAMS][THREADS_PER_TEAM];
	struct thread_params tps[NUM_TEAMS][THREADS_PER_TEAM];
	
	// Lets prepare signal handling for main
	// Block all signals for main until the threads are created
	printf("Main: Blocking all signals ...\n");
	sigfillset(&to_block_main);
	sigprocmask(SIG_BLOCK, &to_block_main, NULL);

	printf("Main: Creating Threads ...\n");
	printf("Main: Unblocking ... Threads will handle it from now on ...\n");
	printf("Main: THREADS WILL SLEEP FOR 30 SECONDS\n\n");
	sigprocmask(SIG_UNBLOCK, &to_block_main, NULL);
	
	// deploy threads!
	for(int i = 0; i < NUM_TEAMS; i++){
		for(int j = 0; j < THREADS_PER_TEAM; j++){
			tps[i][j].team_no = i;
			pthread_create(&thread_ids[i][j], NULL, pthread_func, &tps[i][j]);
		}
	}

	for(int i = 0; i < NUM_TEAMS; i++){
		for(int j = 0; j < THREADS_PER_TEAM; j++){
			pthread_join(thread_ids[i][j], NULL);
		}
	}
	printf("\nMain: Threads joined ...\n");
	signal(SIGINT, SIG_DFL);
	signal(SIGILL, SIG_DFL);
	signal(SIGABRT, SIG_DFL);
	signal(SIGCHLD, SIG_DFL);
	signal(SIGSEGV, SIG_DFL);
	signal(SIGFPE, SIG_DFL);
	signal(SIGHUP, SIG_DFL);
	signal(SIGTSTP, SIG_DFL);
	printf("Main: Restored Default Signal Handling ...\n");
	printf("Main: Sleeping for 20 seconds ...\n");
	sleep(20);	
	
	return 0;
}	