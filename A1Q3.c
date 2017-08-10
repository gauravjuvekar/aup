#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
/*
O_APPEND flag forces file pointer to point at the end of file whenever any operation is performed. 
So if you do lseek from start of the file, it takes the updated file pointer position as a start of file i.e. end position of old file.
*/

int main(){
	
	int fd = open("f1", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	if(fd == -1){
		perror("open failed");
		return errno;
	}
	
	write(fd, "abcde", 5);
	close(fd);
	
	fd = open("f1", O_WRONLY | O_APPEND);
	lseek(fd, 0, SEEK_SET);
	write(fd, "12345", 5);
	
	close(fd);

}

