#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(){

	int fd;
	char *fifo = "/tmp/makfifo";
	
	mkfifo(fifo, 0666);
	
	int i;
	
	for(i = 0; i < 5; i++){
	
		int fd2;
		fd = open(fifo, O_RDONLY);
		char arr[2];
		//arr[1] = '\n';
		read(fd, arr, 2 * sizeof(char));
		//sleep(1);
		fd2 = open("NUMBERS.txt", O_WRONLY| O_CREAT| O_APPEND, 0777);
		write(fd2, arr, 2 * sizeof(char));
		//printf("%d\n", arr[0]);	
		close(fd);
		close(fd2);
		
		
		fd = open(fifo, O_WRONLY);
		arr[0] = ((i * 2) + 1) + 48;
		arr[1] = '\n';
		write(fd, arr, 2 * sizeof(char));
		close(fd);
	}
	return 0;
}
