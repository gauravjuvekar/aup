#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
//#define major(dev) ((long)(((unsigned long) (dev) >> 8) & 0xff))
//#define minor(dev) ((long)((dev) & 0xff))
main(int argc, char *argv[])
{
	struct stat sb;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if (stat(argv[1], &sb) == -1) {
		perror("stat");
		exit(EXIT_FAILURE);
	}

	printf("File type:                ");

	switch (sb.st_mode & S_IFMT) {
		case S_IFBLK:  printf("block device\n");            break;
		case S_IFCHR:  printf("character device\n");        break;
		case S_IFDIR:  printf("directory\n");               break;
		case S_IFIFO:  printf("FIFO/pipe\n");               break;
		case S_IFLNK:  printf("symlink\n");                 break;
		case S_IFREG:  printf("regular file\n");            break;
		case S_IFSOCK: printf("socket\n");                  break;
		default:       printf("unknown?\n");                break;
	}

	printf("I-node number:            %ld\n", (long) sb.st_ino);

	printf("Mode:                     %lo (octal)\n",(unsigned long) sb.st_mode);

	printf("Link count:               %ld\n", (long) sb.st_nlink);
	
	printf("dev no.:                  %ld\n", (long) sb.st_dev);

	printf("dev id:                   %ld\n", (long) sb.st_rdev);

	if(((sb.st_mode & S_IFMT) == S_IFBLK) || ((sb.st_mode & S_IFMT) == S_IFCHR)){
		printf("Major No:                 %ld\n", (long) major(sb.st_rdev));
		printf("Minor No:                 %ld\n", (long) minor(sb.st_rdev));
	}
	exit(EXIT_SUCCESS);
}

