#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>

int main() {
	if (!fork()) {
        mode_t curr_mask;
        curr_mask = umask(0); /*set umask to random value*/
		printf("Umask %4o\n", curr_mask);
        return 0;
	}
}
