#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
void sig_alr(int sig){
	if(sig == SIGALRM)
	fprintf(stderr,"Interrupt caught in parent\n");
}

void sig_alr2(int sig){
	if(sig == SIGALRM)
	fprintf(stderr,"Interrupt caught in child\n");
}

int main() {


	pid_t pid;
	alarm(2);
	if((pid = fork()) == 0){
		signal(SIGALRM, &sig_alr2);
		//kill(getppid(), SIGINT);
	}
	else{
		wait();
		signal(SIGALRM, &sig_alr);
		sleep(5);
	}
	return 0;
}
