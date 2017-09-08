#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

	struct stat sb1, sb2;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
		return -1;
	}

	if (stat(argv[1], &sb1) == -1) {
		perror("stat");
		return -1;
	}

	printf("After first stat\n");
	printf("Last status change:       %s", ctime(&sb1.st_ctime));
	printf("Last file access:         %s", ctime(&sb1.st_atime));
	printf("Last file modification:   %s", ctime(&sb1.st_mtime));


	printf("\n\n");

	if (stat(argv[1], &sb2) == -1) {
		perror("stat");
		return -1;
	}

	sleep(10);

	printf("After second stat (after 10 seconds)\n");
	printf("Last status change:       %s", ctime(&sb2.st_ctime));
	printf("Last file access:         %s", ctime(&sb2.st_atime));
	printf("Last file modification:   %s", ctime(&sb2.st_mtime));

	printf("\n\n");
	
	if(ctime(&sb1.st_ctime) != ctime(&sb2.st_ctime))
		printf("Change time has changed\n");
	else
		printf("Change time has not changed\n");
	
	if(ctime(&sb1.st_atime) != ctime(&sb2.st_atime))
		printf("Access time has changed\n");
	else
		printf("Access time has not changed\n");

	if(ctime(&sb1.st_mtime) != ctime(&sb2.st_mtime))
		printf("Modify time has changed\n");
	else
		printf("Modify time has not changed\n");

	return 0;


}
