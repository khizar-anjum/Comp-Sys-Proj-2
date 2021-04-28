#include "utils.h"

void simple_sighandler(int signum, siginfo_t *info, void *ptr)	{
	printf("Main: process %d and I am handling signal %d\n", getpid(), signum);
	printf("Signal originates from process %lu\n", (unsigned long)info->si_pid);
}

void thread1_sighandler(int signum, siginfo_t *info, void *ptr) {
	printf("Thread 1: I am handling signal %d\n", signum);
	printf("Signal originates from process %lu\n", (unsigned long)info->si_pid);	
}

void thread2_sighandler(int signum, siginfo_t *info, void *ptr) {
	printf("Thread 2: I am handling signal %d\n", signum);
	printf("Signal originates from process %lu\n", (unsigned long)info->si_pid);	
}

void thread3_sighandler(int signum, siginfo_t *info, void *ptr) {
	printf("Thread 3: I am handling signal %d\n", signum);
	printf("Signal originates from process %lu\n", (unsigned long)info->si_pid);	
}

void thread4_sighandler(int signum, siginfo_t *info, void *ptr) {
	printf("Thread 4: I am handling signal %d\n", signum);
	printf("Signal originates from process %lu\n", (unsigned long)info->si_pid);	
}

void *pthread_func(void* vargp){
	struct thread_params* tps = (struct thread_params*) vargp;
	struct sigaction t_params;
	int success;
	t_params.sa_flags = SA_SIGINFO; // | SA_RESTART
	sigset_t to_block_thread;
	sigfillset(&to_block_thread);

	// check which threads to deploy (ASSUMING 4 TEAMS)
	switch(tps->team_no){
		case 0:
			printf("Thread %d: Handling SIGINT, SIGSEGV, SIGFPE ...\n", tps->team_no);
			t_params.sa_sigaction = thread1_sighandler;
			sigdelset(&to_block_thread, SIGINT); 
			sigdelset(&to_block_thread, SIGSEGV);
			sigdelset(&to_block_thread, SIGFPE);
			sigprocmask(SIG_BLOCK, &to_block_thread, NULL);
			success = sigaction(SIGINT, &t_params, NULL);
			success = sigaction(SIGSEGV, &t_params, NULL);
			success = sigaction(SIGFPE, &t_params, NULL);
			break;
		case 1:
			printf("Thread %d: Handling SIGABRT, SIGFPE, SIGHUP ...\n", tps->team_no);
			t_params.sa_sigaction = thread2_sighandler;
			sigdelset(&to_block_thread, SIGABRT); 
			sigdelset(&to_block_thread, SIGFPE);
			sigdelset(&to_block_thread, SIGHUP);
			sigprocmask(SIG_BLOCK, &to_block_thread, NULL);
			success = sigaction(SIGABRT, &t_params, NULL);
			success = sigaction(SIGFPE, &t_params, NULL);
			success = sigaction(SIGHUP, &t_params, NULL);
			break;
		case 2:
			printf("Thread %d: Handling SIGILL, SIGHUP, SIGTSTP ...\n", tps->team_no);
			t_params.sa_sigaction = thread3_sighandler;
			sigdelset(&to_block_thread, SIGILL); 
			sigdelset(&to_block_thread, SIGHUP);
			sigdelset(&to_block_thread, SIGTSTP);
			sigprocmask(SIG_BLOCK, &to_block_thread, NULL);
			success = sigaction(SIGILL, &t_params, NULL);
			success = sigaction(SIGHUP, &t_params, NULL);
			success = sigaction(SIGTSTP, &t_params, NULL);
			break;
		case 3:
			printf("Thread %d: Handling SIGCHLD, SIGSEGV, SIGTSTP ...\n", tps->team_no);
			t_params.sa_sigaction = thread4_sighandler;
			sigdelset(&to_block_thread, SIGCHLD); 
			sigdelset(&to_block_thread, SIGSEGV);
			sigdelset(&to_block_thread, SIGTSTP);
			sigprocmask(SIG_BLOCK, &to_block_thread, NULL);
			success = sigaction(SIGCHLD, &t_params, NULL);
			success = sigaction(SIGSEGV, &t_params, NULL);
			success = sigaction(SIGTSTP, &t_params, NULL);
			break;
		default:
			printf("Invalid Thread!\n");
			pthread_exit(0);
			break;
	}

	for(int i = 0; i < 30; i++) sleep(1);
}
