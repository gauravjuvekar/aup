#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<stdlib.h>
#include<setjmp.h>

static jmp_buf env_alrm;

static void sig_alrm(int sig){

	longjmp(env_alrm, 1);
}

unsigned int sleep2(unsigned int nsecs){

	if(signal(SIGALRM, sig_alrm) == SIG_ERR)
		return(nsecs);
	if(setjmp(env_alrm) == 0){
		alarm(nsecs);	// start timer
		pause();
	}
	return(alarm(0)); //turn off timer and return unslept time
}

static void sig_int(int sig){

	int i;
	volatile int j;
	printf("\nsig_int starting\n");
	for(i = 0; i < 200000; i++)
		j += i + i;
	printf("sig_int fin\n");
	return;

}

int main(){

	unsigned int unslept;

	if(signal(SIGINT, sig_int) == SIG_ERR){
		printf("signal(SIGINT) error");
	}

	unslept = sleep2(5);
	printf("sleep2 returned: %u\n", unslept);

	exit(0);
}

