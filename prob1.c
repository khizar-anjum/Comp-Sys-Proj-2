#include "utils.h"

#define TEAMS 4
#define THREADS_PER_TEAM 1


int main(int argc, char* argv[]){
	printf("Porting CRTL-C control to default sighandler\n");

	struct sigaction my_params;
	int success;

	my_params.sa_sigaction = simple_sighandler;
	my_params.sa_flags = SA_SIGINFO; // | SA_RESTART
	sigemptyset(&my_params.sa_mask);

	success = sigaction(SIGINT, &my_params, NULL);

	printf("Check it out\n");
	while(1)
    {
        sleep(1);
    }
	
	return 0;
}	