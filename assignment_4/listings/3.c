#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/time.h>
#include<inttypes.h>
#include<time.h>
#include<stdlib.h>

/* Using clock_gettime(2) instead of gettimeofday(2) as clock_gettime has a
 * better resolution (in nanoseconds)
 */


int main() {
	int i;
	struct timespec start;
	struct timespec end;
	uint64_t sum = 0;

	for (i = 0; i < 10; i++) {
		clock_gettime(CLOCK_REALTIME, &start);
		volatile pid_t pid = getpid(); /* volatile so that it's not optimzed */
		clock_gettime(CLOCK_REALTIME, &end);
		(void)pid;
		uint64_t diff = (end.tv_sec - start.tv_sec) * 1000000000 +
			   (end.tv_nsec - start.tv_nsec);
		sum += diff;
	}

	uint64_t average = sum / 10;
	printf("Average time for getpid(): %" PRIu64 "ns\n", average);

	int pipes[2];
	sum = 0;
	for (i = 0; i < 10; i++) {
		pipe(pipes);
		clock_gettime(CLOCK_REALTIME, &start);
		volatile int forkval = fork(); /* volatile so that it's not optimzed */
		clock_gettime(CLOCK_REALTIME, &end);
		uint64_t diff = (end.tv_sec - start.tv_sec) * 1000000000 +
			   (end.tv_nsec - start.tv_nsec);
		/* Use only the smaller value of the two (which will usually be the one
		 * who was scheduled to run earlier) */
		if (forkval) {
			/* parent */
			close(pipes[1]);
			uint64_t diff_child;
			read(pipes[0], &diff_child, sizeof(diff_child));
			if (diff < diff_child){
				sum += diff;
			}
			else {
				sum += diff_child;
			}
			close(pipes[0]);
		}
		else {
			/* child */
			close(pipes[0]);
			write(pipes[1], &diff, sizeof(diff));
			exit(0);
		}
	}
	average = sum / 10;
	printf("Average time for fork(): %" PRIu64 "ns\n", average);
}
