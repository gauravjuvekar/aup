#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include <sys/sysinfo.h>
#include <unistd.h>

int wait2(int *wtime, int *rtime, int *iotime);


int wait2(int *wtime, int *rtime, int *iotime) {
	siginfo_t infop;
	int ret = waitid(P_ALL, 0, &infop, WEXITED | WNOWAIT);
	/* Use WNOWAIT to keep the process in a waitable state so that we can gather
	 * process stats from /proc/pid/stat */
	if (ret == -1) {
		fprintf(stderr, "waitid() error\n");
		return -1;
	}
	char pid_path[PATH_MAX + 1];
	sprintf(pid_path, "/proc/%d/stat", infop.si_pid);
	FILE *pidstat = fopen(pid_path, "r");
	if (pidstat == NULL) {
		fprintf(stderr, "fopen() error\n");
		return -1;
	}
	unsigned long int utime;
	unsigned long int stime;
	unsigned long long int delayacct_blkio_tics;
	unsigned long long int starttime;
	ret = fscanf(
		pidstat,
		"%*d"  /* pid */    " %*s" /* comm */    " %*c" /* state */
		" %*d" /* ppid */   " %*d" /* pgrp */    " %*d" /* session */
		" %*d" /* tty_nr */ " %*d" /* tpgid */   " %*u" /* flags */
		" %*u" /* minflt */ " %*u" /* cminflt */ " %*u" /* majflt */
		" %*u" /* cmajflt */
		" %lu" /* <=================  utime */
		" %lu" /* <=================  stime */
		" %*d" /* cutime */ " %*d" /* cstime */ " %*d" /* priority */
		" %*d" /* nice */   " %*d" /* num_threads */
		" %*d" /* itrealvalue */
		" %llu" /* <=================  starttime */
		" %*u" /* vsize */       " %*d" /* rss */       " %*u" /* rsslim */
		" %*u" /* startcode */   " %*u" /* endcode */   " %*u" /* startstack */
		" %*u" /* kstkesp */     " %*u" /* kstkeip */   " %*u" /* signal */
		" %*u" /* blocked */     " %*u" /* sigignore */ " %*u" /* sigcatch */
		" %*u" /* wchan */       " %*u" /* nswap */     " %*u" /* cnswap */
		" %*d" /* exit_signal */ " %*d" /* processor */ " %*u" /* rt_priority */
		" %*u" /* policy */
		" %llu" /* <=================  delayacct_blkio_tics */
		" %*u" /* guest_time */ " %*d" /* cguest_time */
		" %*u" /* start_data */ " %*u" /* end_data */ " %*u" /* start_brk */
		" %*u" /* arg_start */  " %*u" /* arg_end */  " %*u" /* env_start */
		" %*u" /* env_end */    " %*u" /* exit_code */
		, &utime, &stime, &starttime, &delayacct_blkio_tics);

	if (ret != 4) {
		fprintf(stderr, "fscanf() error, matched %d\n", ret);
		fclose(pidstat);
		return -1;
	}
	fclose(pidstat);

	struct sysinfo current_info;
	if (sysinfo(&current_info)) {
		fprintf(stderr, "sysinfo() error\n");
		return -1;
	}
	unsigned long long int uptime = current_info.uptime * sysconf(_SC_CLK_TCK);
	*rtime = utime + stime;
	*iotime = delayacct_blkio_tics;
	*wtime = uptime - starttime - *rtime - *iotime;

	return waitpid(infop.si_pid, NULL, 0);
}


int main() {
	int pid;
	if ((pid = fork()) > 0) {
		int wtime, iotime, rtime;
		int pid_wait = wait2(&wtime, &rtime, &iotime);
		assert(pid_wait == pid);
		printf("W %d, R %d, IO %d\n", wtime, rtime, iotime);
	}
	else {
		volatile unsigned long long int i;
		for (i = 0; i < 1000000000; i++);
		sleep(1);
	}
}
