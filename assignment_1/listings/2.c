#define _GNU_SOURCE 1
#define _XOPEN_SOURCE 500
#define _POSIX_C_SOURCE 201112L
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MIN(a,b) \
	({ typeof (a) _a = (a); \
	   typeof (b) _b = (b); \
	   _a < _b ? _a : _b; })


int main(int argc, char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Usage: ./truncate LENGTH FILE\n");
		return 1;
	}
	const char *file_name = argv[2];

	char *length_end;
	off_t truncate_to;
	truncate_to = strtoul(argv[1], &length_end, 10);
	if (*length_end) {
		fprintf(stderr, "Invalid length %s\n", argv[1]);
		return 1;
	}

	struct stat file_stat;
	errno = 0;
	if (stat(file_name, &file_stat) == -1) {
		fprintf(stderr, "./truncate: %s: %s\n", file_name, strerror(errno));
		return 1;
	}

	char copy_buf[file_stat.st_blksize];
	if (file_stat.st_size < truncate_to) {
		errno = 0;
		int fd = open(file_name, O_WRONLY | O_APPEND);
		if (fd == -1) {
			fprintf(stderr, "./truncate: %s: %s\n", file_name, strerror(errno));
			return 1;
		}

		memset(copy_buf, 0, sizeof(copy_buf));
		truncate_to -= file_stat.st_size;
		while (truncate_to) {
			const ssize_t to_write = MIN(truncate_to, sizeof(copy_buf));
			errno = 0;
			const ssize_t written = write(fd, copy_buf, to_write);
			if (written != to_write) {
				if (errno) {
					fprintf(stderr, "./truncate: write: %s\n", strerror(errno));
				}
				else {
					fprintf(stderr, "./truncate: write: less bytes written\n");
				}
				return 1;
			}
			truncate_to -= written;
		}
		errno = 0;
		if (close(fd) == -1) {
			fprintf(stderr, "./truncate: close: %s\n", strerror(errno));
			return 1;
		}
	}
	else if (file_stat.st_size > truncate_to){
		char abs_dir_path[PATH_MAX + 1];
		errno = 0;
		if (realpath(file_name, abs_dir_path) == NULL) {
			fprintf(stderr, "./truncate: close: %s\n", strerror(errno));
			return 1;
		}

		int len = strlen(abs_dir_path);
		while (abs_dir_path[len - 1] != '/') len--;
		abs_dir_path[len] = '\0';

		errno = 0;
		int new = open(abs_dir_path, O_WRONLY | O_TMPFILE, file_stat.st_mode);
		if (new == -1) {
			fprintf(stderr, "./truncate: %s\n", strerror(errno));
			return 1;
		}

		errno = 0;
		int old = open(file_name, O_RDONLY);
		if (old == -1) {
			fprintf(stderr, "./truncate: %s: %s\n", file_name, strerror(errno));
			return 1;
		}

		while (truncate_to) {
			const ssize_t to_copy = MIN(truncate_to, file_stat.st_blksize);
			errno = 0;
			const ssize_t n_read = read(old, copy_buf, to_copy);
			if (n_read != to_copy) {
				if (errno) {
					fprintf(stderr, "./truncate: read: %s\n", strerror(errno));
				}
				else {
					fprintf(stderr, "./truncate: read: less bytes written\n");
				}
				return 1;
			}
			errno = 0;
			const ssize_t n_written = write(new, copy_buf, n_read);
			if (n_written != n_read) {
				if (errno) {
					fprintf(stderr, "./truncate: write: %s\n", strerror(errno));
				}
				else {
					fprintf(stderr, "./truncate: write: less bytes written\n");
				}
				return 1;
			}
			truncate_to -= n_written;
		}

		errno = 0;
		if (close(old) == -1) {
			fprintf(stderr, "./truncate: close: %s\n", strerror(errno));
			return 1;
		}

		if (unlink(file_name) == -1) {
			fprintf(stderr, "./truncate: unlink: %s\n", strerror(errno));
			return 1;
		}

		char fd_path[PATH_MAX + 1];
		snprintf(fd_path, PATH_MAX, "/proc/self/fd/%d", new);
		if (linkat(AT_FDCWD, fd_path,
					AT_FDCWD, file_name, AT_SYMLINK_FOLLOW) == -1) {
			fprintf(stderr, "./truncate: linkat: %s\n", strerror(errno));
			return 1;
		}
	}
	return 0;
}
