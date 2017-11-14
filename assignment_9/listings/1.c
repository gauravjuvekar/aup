#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


void sigterm_handler(int signum) {
	printf("SIGTERM (%d), caught\n", signum);
}

int main() {
	signal(SIGINT, SIG_IGN);
	signal(SIGSEGV, SIG_DFL);
	signal(SIGTERM, sigterm_handler);

	pause();

	struct sigaction action;

	memset(&action, 0, sizeof(action));
	action.sa_handler = SIG_IGN;
	sigaction(SIGINT, &action, NULL);

	memset(&action, 0, sizeof(action));
	action.sa_handler = SIG_DFL;
	sigaction(SIGSEGV, &action, NULL);

	memset(&action, 0, sizeof(action));
	action.sa_handler = sigterm_handler;
	sigaction(SIGTERM, &action, NULL);

	pause();
}
