#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
int main() {
       
       pid_t pid;
       int fd;
       if((pid = fork()) == 0){
           setsid();
           printf("PID : %d, PGID : %d of child process.\n",(int)getpid(), (int)getpgid(getpid()));
           if(getpid() == getpgid(getpid()))
               printf("Process is group leader\n"); 
           if((fd = open("/dev/tty")) == -1)
               printf("/dev/tty failed to open. No controlling terminal.\n"); 
       }
       else
           exit(0);
       return 0;
}

