#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define PROGNAME "./readlinkdir"


void resolve_link(const char *pathname, char linkbuf[static PATH_MAX + 1]) {
	struct stat statbuf;
	char base_str[PATH_MAX + 1];
	char dir_str[PATH_MAX + 1];
	strcpy(linkbuf, pathname);

	char orig_dir[PATH_MAX + 1];
	errno = 0;
	getcwd(orig_dir, sizeof(orig_dir));
	if (errno) {
		fprintf(stderr, PROGNAME": getcwd: %s\n", strerror(errno));
		exit(1);
	}

	do {
		strcpy(base_str, basename(linkbuf));
		strcpy(dir_str, dirname(linkbuf));
		errno = 0;
		if (chdir(dir_str)) {
			fprintf(stderr, PROGNAME": chdir: %s: %s\n",
			        dir_str, strerror(errno));
			exit(1);
		}

		char abs_cwd[PATH_MAX + 1];
		getcwd(abs_cwd, sizeof(abs_cwd));
		if (errno) {
			fprintf(stderr, PROGNAME": getcwd: %s\n", strerror(errno));
			exit(1);
		}
		strcpy(dir_str, abs_cwd);

		errno = 0;
		if (lstat(base_str, &statbuf)) {
			fprintf(stderr, PROGNAME": lstat: %s/%s: %s\n",
			        abs_cwd, base_str, strerror(errno));
			exit(1);
		}

		if ((statbuf.st_mode & S_IFMT) == S_IFLNK) {
			errno = 0;
			ssize_t read_len = readlink(base_str, linkbuf, PATH_MAX);
			if (errno) {
				fprintf(stderr, PROGNAME": readlink: %s/%s: %s\n",
						abs_cwd, base_str, strerror(errno));
				exit(1);
			}
			linkbuf[read_len] = '\0';
		}
	} while ((statbuf.st_mode & S_IFMT) == S_IFLNK);
	snprintf(linkbuf, PATH_MAX + 1, "%s/%s", dir_str, base_str);

	if (chdir(orig_dir)) {
		fprintf(stderr, PROGNAME": chdir: %s: %s\n",
				orig_dir, strerror(errno));
		exit(1);
	}
}

int main(int argc, char *argv[]) {
	if (argc > 2) {
		fprintf(stderr, "Usage: "PROGNAME"[DIR]\n");
		exit(1);
	}

	char *dir_path;
	if (argc == 2) {
		dir_path = argv[1];
	}
	else {
		dir_path = ".";
	}

	errno = 0;
	size_t dir_path_len = strlen(dir_path);
	while (dir_path[dir_path_len - 1] == '/') {
		dir_path[dir_path_len - 1] = '\0';
		dir_path_len -= 1;
	}

	DIR *dir = opendir(dir_path);
	if (errno || (dir == NULL)) {
		fprintf(stderr, PROGNAME": open: %s\n", strerror(errno));
		exit(1);
	}

	struct dirent *dir_entry;
	char target_path[PATH_MAX + 1];
	while ((dir_entry = readdir(dir)) != NULL) {

		struct stat statbuf;
		char full_path[PATH_MAX + 1];
		snprintf(full_path, sizeof(full_path), "%s/%s",
		         dir_path, dir_entry->d_name);

		errno = 0;
		if (lstat(full_path, &statbuf)) {
			fprintf(stderr, PROGNAME": lstat: %s: %s\n",
			        full_path, strerror(errno));
			exit(1);
		}

		if ((statbuf.st_mode & S_IFMT) == S_IFLNK) {
			resolve_link(full_path, target_path);
			printf("%s -> %s\n", full_path, target_path);
		}
	}

	return 0;
}
