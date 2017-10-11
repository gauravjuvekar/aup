#define _POSIX_C_SOURCE 1
#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


const size_t READ_SIZE = 512;

volatile bool quit_event = false;


bool search_word_file(const char *search_word, int file) {
	char buffer[2 * READ_SIZE + 1];
	ssize_t n_read = 0;
	size_t read_pos = 0;
	char *saveptr = NULL;
	while ((n_read = read(file, &buffer[read_pos], READ_SIZE)) > 0) {
		buffer[read_pos + n_read + 1] = '\0';
		char *word = NULL;
		char *last_word = word;
		char *next_buf = buffer;
		do {
			word = strtok_r(next_buf, " ,.;/\t\n()[]{}<>'\"*#$%^&-_=+@!",
			                &saveptr);
			if (word == NULL) {
				break;
			}
			next_buf = NULL;
			if (strcmp(word, search_word) == 0) {
				/* Return only the first match */
				return true;
			}
			last_word = word;
		} while (!quit_event && word != NULL);
		const size_t last_len = strlen(last_word);
		memmove(buffer, last_word, last_len + 1);
		buffer[last_len + 1] = ' '; /* so that it's not '\0' */
		read_pos = last_len + 1;
	}
	return false;
}

typedef struct {
	const char *word;
	const char *filename;
} PthreadArgs;


void *search(void *arg) {
	PthreadArgs *args = (PthreadArgs *)arg;
	int file = open(args->filename, O_RDONLY);
	if (file == -1) {
		fprintf(stderr, "Cannot open %s\n", args->filename);
		pthread_exit(NULL);
	}
	if (search_word_file(args->word, file)) {
		printf("%s: %s\n", args->filename, args->word);
		/* Stop searching in other files if a match is found
		 * It is sometimes possible that other threads will also print, but we
		 * ignore it as quitting is just for efficiency. */
		quit_event = true;
		pthread_exit((char *)(args->filename));
	}
	else {
		pthread_exit(NULL);
	}
}



int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Usage: ./search_word_multi WORD [FILE] ...\n");
		exit(1);
	}
	const char *word = argv[1];

	const size_t n_files = argc - 2;
	PthreadArgs all_args[n_files];
	pthread_t threads[n_files];

	size_t i = 0;
	for (i = 0; i < n_files; ++i) {
		all_args[i].word = word;
		all_args[i].filename = argv[i + 2];
		/* Full memory barrier  */
		__sync_synchronize();

		if (pthread_create(&threads[i], NULL, search, &all_args[i])) {
			fprintf(stderr, "pthread_create failed\n");
			exit(1);
		}
	}

	for (i = 0; i < n_files; i++) {
		void *retval;
		if (pthread_join(threads[i], &retval)) {
			fprintf(stderr, "pthread_join failed\n");
		}
	}

	return 0;
}
