#define _POSIX_C_SOURCE 200809L
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<time.h>

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: ./3 number\n");
		return 1;
	}

	const int count = atoi(argv[1]);
	if (count < 0) {
		fprintf(stderr, "Count should be positive\n");
	}

	int i;
	pid_t pid;
	for (i = 0; i < count; ++i) {
		if ((pid = fork())) {
		}
		else {
			/* wait in multiples of 100 ms which seems to be a decent value */
			long long int nanosec_to_wait = 1000 * 1000 * 100 * count;
			long long int seconds_to_wait = nanosec_to_wait / 1000000000;
			nanosec_to_wait %= 1000000000;
			struct timespec wait_time = {.tv_sec = seconds_to_wait,
				                         .tv_nsec = nanosec_to_wait};
			nanosleep(&wait_time, NULL);
			exit(0);
		}
	}

	for (i = 0; i < count; ++i) {
		int status;
		do {
			wait(&status);
		} while (!WIFEXITED(status));
	}
	return 0;
}
