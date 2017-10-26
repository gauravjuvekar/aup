#include<stdio.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	int fd = open(argv[1], O_RDONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
	int fd1 = open(argv[2], O_WRONLY | O_CREAT , S_IRUSR | S_IWUSR);
	
	if(argv[1] == NULL)
		printf("Enter source file\n");
	if(argv[2] == NULL)
		printf("Enter destination file\n");

	if(fd == -1){
		perror("open failed");
		return errno;
	}
	if(fd1 == -1){
		perror("open failed");
		return errno;
	}


	char arr[16];
	
	while(read(fd, arr, 1)){
		write(fd1, arr, 1);
	}

	close(fd);
	close(fd1);

	return 0;

}
