#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>

extern int errno;

int main(int argc, char *argv[]){
    int fd, pid;
    char buf[6];
    fd = atoi(argv[1]);         /*File descriptor opened in parent*/
    int n = read(fd, buf, 5);
    buf[5] = '\0';
    if(n == -1){
        fprintf(stderr, "Error opening file : %s\n", strerror(errno));
    }
    else
        printf("%s\n",buf);
    return 0;
}
