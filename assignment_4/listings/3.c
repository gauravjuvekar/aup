#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/time.h>
#include<inttypes.h>
#include<time.h>

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
		volatile pid_t pid = getpid();
		clock_gettime(CLOCK_REALTIME, &end);
		(void)pid;
		uint64_t diff = (end.tv_sec - start.tv_sec) * 1000000000 +
			   (end.tv_nsec - start.tv_nsec);
		sum += diff;
	}

	uint64_t average = sum / 10;
	printf("Average time for getpid(): %" PRIu64 "ns\n", average);

}
