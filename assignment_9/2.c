#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
void sig_usr(int sig){
    fprintf(stderr,"Interrupt caught in parent\n");
}
int main() {
    pid_t pid;
    if((pid = fork()) == 0){
        kill(getppid(), SIGINT);
    }
    else{
        signal(SIGINT, &sig_usr);
        sleep(5);
    }
    return 0;
}
