#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
int main(){

//demonstration that a child process inherits 
//real user id, real group id, effective user id and effective group id of the parent process
//while process id and parent process id are not.
	
	if(fork() == 0){
		// gives the details of the child process	
		printf("This is the child process\n");
		printf("the pid is: %d\n", getpid());
		printf("the parent pid is: %d\n", getppid());
		printf("the real uid is : %d\n", getuid());
		printf("the efective uid is : %d\n", geteuid());
		printf("the real gid is : %d\n", getgid());
		printf("the effective gid is : %d\n\n", getegid());
	}

	else{
		// gives the details of the parent process
		printf("This is the parent process\n");
		printf("the pid is: %d\n", getpid());
		printf("the parent pid is: %d\n", getppid());
		printf("the real uid is : %d\n", getuid());
		printf("the efective uid is : %d\n", geteuid());
		printf("the real gid is : %d\n", getgid());
		printf("the effective gid is : %d\n\n", getegid());
	}
		
	return 0;

}


