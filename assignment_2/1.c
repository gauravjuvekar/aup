#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>

void listall(char *dirname){

	DIR*p;
	struct dirent *d;
	p=opendir(dirname);
	if(p==NULL)
	{
		perror("Cannot find directory");
		//		exit(-1);
		return;
	}
	while(d=readdir(p))
		printf("%s\n",d->d_name);


}

int main(int argc, const char *argv[])
{
	if (mkdir("Junk",0777) == -1) {
		perror(argv[0]);
		exit(EXIT_FAILURE);
	}

	int i;
	char p = '1';
	for(i = 1; i <= 5; i++){
		char str[64]; //Large enough buffer.

		snprintf(str, sizeof(str), "./Junk/%d", i);

		int fd = open(str, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

		if(fd == -1){
			perror("open failed");
			return errno;
		}

		write(fd, "hello", 5);
		close(fd);
	}

	listall("Junk");

	chmod("Junk", 0333);

	listall("Junk");

	chmod("Junk", 0777);

	listall("Junk");

	chmod("Junk", 0666);

	chdir("Junk");

	chmod("Junk", 0777);

	chdir("Junk");


	return 0;
}
