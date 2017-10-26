#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* for fork */
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */
#include <fcntl.h>
#include <errno.h>
int main(int argc, char *argv[]){

	char arr[16];
	if(argv[1] == NULL){
		printf("Enter filename");
	}

	int fd = open(argv[1], O_RDONLY,  S_IRUSR | S_IWUSR);
	
	if(fd == -1){
		perror("open failed");
		return errno;
	}

	int len;
	if(len = read(fd, arr, 16) < 0){
		printf("read error\n");
		return 0;
	}	


	if(arr[len - 1] == '\n')
		arr[len - 1] = '\0';
	else
		arr[len] = '\0';
	
	int pid = fork();
	if(pid < 0){
		printf("Error");
		return 0;
	}
	if (pid==0) { /* child process */
		static char *tab[]={"gcc","a.c",NULL};
		execvp("gcc",tab);
		exit(127); /* only if execv fails */
	}

}
