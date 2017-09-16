#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
int main(int argc, char *argv[]) {

        int ret, fd1, n;
        char buff[6];
        fd1 = open(argv[1], O_RDONLY);
        if(fork() == 0){
            n = read(fd1, buff, 5);
            buff[n] = '\0';
            printf("%s\n", buff);
        }
        return 0;
}
