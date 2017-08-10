#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

size_t getFilesize(const char* filename) {
	struct stat st;
	if(stat(filename, &st) != 0) {
		return 0;
	}
	return st.st_size;   
}

int main(){

	int fd = open("f1", O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
	int len = getFilesize("f1");

	if(fd == -1){
		perror("open failed");
		return errno;
	}
	char a[10];

	read(fd, a, 10);
//	int i;
//	for(i = 0; i < 10; i++)
//		printf("%c", a[i]);
	
	write(fd, "hello", 5);
	close(fd);


}

