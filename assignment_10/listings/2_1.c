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
	
		fd = open(fifo, O_WRONLY | O_CREAT);
		
		char num[2]; 
		num[0]= (i * 2) + 48;
		num[1] = '\n'; 
		write(fd, num, 2 * sizeof(char));
		close(fd);
		
		fd = open(fifo, O_RDONLY);
		
		char num2[2];
		num2[1] = '\n';
		read(fd, num2, 2 * sizeof(char));
		
		int fd2;
		fd2 = open("NUMBERS.txt", O_WRONLY| O_CREAT| O_APPEND, 0777);
		write(fd2, num2, 2*sizeof(char));
		//sleep(1);
		//printf("%d\n", num2[0]);
		close(fd);
		close(fd2);	
	}
	return 0;
}
