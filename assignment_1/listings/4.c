#define _GNU_SOURCE 1
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(__attribute__((unused)) int argc,
         __attribute__((unused)) char *argv[]) {
	errno = 0;
	int fd = open("./cheese.bin",
	              O_CREAT | O_TRUNC | O_WRONLY,
	              S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

	if (fd == -1) {
		fprintf(stderr, "./holey_files: open: %s\n", strerror(errno));
		return 1;
	}

	errno = 0;
	if (fallocate(fd, 0, 0, 40) == -1) {
		fprintf(stderr, "./holey_files: fallocate: %s\n", strerror(errno));
		return 1;
	};
	errno = 0;
	if (lseek(fd, 10, SEEK_SET) == (off_t)(-1)) {
		fprintf(stderr, "./holey_files: lseek: %s\n", strerror(errno));
		return 1;
	}
	errno = 0;
	if (write(fd, "123456789a", 10) != 10) {
		fprintf(stderr, "./holey_files: write: %s\n", strerror(errno));
		return 1;
	}
	errno = 0;
	if (lseek(fd, 30, SEEK_SET) == (off_t)(-1)) {
		fprintf(stderr, "./holey_files: lseek: %s\n", strerror(errno));
		return 1;
	}
	errno = 0;
	if (write(fd, "bcdefghijk", 10) != 10) {
		fprintf(stderr, "./holey_files: write: %s\n", strerror(errno));
		return 1;
	}
	errno = 0;
	if (close(fd) == -1) {
		fprintf(stderr, "./holey_files: close: %s\n", strerror(errno));
		return 1;
	}


	system("od ./cheese.bin");

	errno = 0;
	fd = open("./cheese.bin", O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "./holey_files: open: %s\n", strerror(errno));
		return 1;
	}

	errno = 0;
	int fd2 = open("./cheese2.bin",
	               O_CREAT | O_TRUNC | O_WRONLY,
	               S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd2 == -1) {
		fprintf(stderr, "./holey_files: open: %s\n", strerror(errno));
		return 1;
	}

	errno = 0;
	if (fallocate(fd2, 0, 0, 40) == -1) {
		fprintf(stderr, "./holey_files: fallocate: %s\n", strerror(errno));
		return 1;
	};

	char buf[10];
	errno = 0;
	if (lseek(fd, 10, SEEK_SET) == (off_t)(-1)) {
		fprintf(stderr, "./holey_files: lseek: %s\n", strerror(errno));
		return 1;
	}
	errno = 0;
	if (read(fd, buf, 10) != 10) {
		fprintf(stderr, "./holey_files: read: %s\n", strerror(errno));
		return 1;
	}
	errno = 0;
	if (lseek(fd2, 10, SEEK_SET) == (off_t)(-1)) {
		fprintf(stderr, "./holey_files: lseek: %s\n", strerror(errno));
		return 1;
	}
	errno = 0;
	if (write(fd2, buf, 10) != 10) {
		fprintf(stderr, "./holey_files: write: %s\n", strerror(errno));
		return 1;
	}

	errno = 0;
	if (lseek(fd, 30, SEEK_SET) == (off_t)(-1)) {
		fprintf(stderr, "./holey_files: lseek: %s\n", strerror(errno));
		return 1;
	}
	errno = 0;
	if (read(fd, buf, 10) != 10) {
		fprintf(stderr, "./holey_files: read: %s\n", strerror(errno));
		return 1;
	}
	errno = 0;
	if (lseek(fd2, 30, SEEK_SET) == (off_t)(-1)) {
		fprintf(stderr, "./holey_files: lseek: %s\n", strerror(errno));
		return 1;
	}
	errno = 0;
	if (write(fd2, buf, 10) != 10) {
		fprintf(stderr, "./holey_files: write: %s\n", strerror(errno));
		return 1;
	}

	errno = 0;
	if (close(fd) == -1) {
		fprintf(stderr, "./holey_files: close: %s\n", strerror(errno));
		return 1;
	}
	errno = 0;
	if (close(fd2) == -1) {
		fprintf(stderr, "./holey_files: close: %s\n", strerror(errno));
		return 1;
	}

	system("od ./cheese2.bin");
}
