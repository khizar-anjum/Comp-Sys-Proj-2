#include "utils.h"

#define NUM_TEAMS 4
#define THREADS_PER_TEAM 1


int main(int argc, char* argv[]){

	// initilize
	struct sigaction my_params;
	int success;
	sigset_t to_block_main;
	sigset_t to_block_threads[NUM_TEAMS];
	pthread_t thread_ids[NUM_TEAMS][THREADS_PER_TEAM];
	struct thread_params tps[NUM_TEAMS][THREADS_PER_TEAM];

	// Lets prepare signal handling for main
	// Block all signals for main until the threads are created
	sigfillset(&to_block_main);
	sigprocmask(SIG_BLOCK, &to_block_main, NULL);

	// Assume we are creating some threads
	printf("I have blocked everything!\n");
	for(int i = 0; i < 10; i++) sleep(1);

	my_params.sa_sigaction = simple_sighandler;
	my_params.sa_flags = SA_SIGINFO; // | SA_RESTART
	sigemptyset(&my_params.sa_mask);

	// success = sigaction(SIGINT, &my_params, NULL);

	// deploy threads!
	for(int i = 0; i < NUM_TEAMS; i++){
		for(int j = 0; j < THREADS_PER_TEAM; j++){
			tps[i][j].team_no = i;
			pthread_create(&thread_ids[i][j], NULL, pthread_func, &tps[i][j]);
		}
	}

	for(int i = 0; i < THREADS_PER_TEAM; i++){
		for(int j = 0; j < NUM_TEAMS; j++){
			pthread_join(thread_ids[i][j], NULL);
		}
	}

	printf("I am unblocking\n");
	sigprocmask(SIG_UNBLOCK, &to_block_main, NULL);
	while(1) sleep(1);
	
	
	return 0;
}	