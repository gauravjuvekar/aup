#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "Usage: ./supercat FILE\n");
		exit(1);
	}

	execl("/bin/cat", "cat", argv[1], NULL);
}
