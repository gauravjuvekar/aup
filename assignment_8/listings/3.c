#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
int main(){
	
	pid_t pid1, pid2;
	int i;
	pid1 = fork();
	if(pid1 < 0){
		printf("fork error");
		return 0;
	}
	else if(pid1 == 0){
		setpgid(0, 0);
		printf("Child..\n");
		printf("before exec\n");
		printf("pid: %d, pgid : %d\n",getpid(), getpgid(getpid()));

		printf("Just before exec");
		static char *tab[]={"gcc","a.c",NULL};
		execvp("gcc",tab);
		exit(127);
	}
	else{
		sleep(1);
		printf("Parent..\n");
		printf("Child's pid, pgid before setpgid\n");
		printf("pid: %d, pgid : %d\n",pid1, getpgid(getpid()));
		if(setpgid(pid1, pid1) == -1){
			printf("after exec\n");
			printf("Error, can't change pgid\n");
		
		}
	}

	return 0;

}
