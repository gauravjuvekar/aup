#include<unistd.h>
#include<stdio.h>
#include<sys/wait.h>
#include<string.h>
int main(){

    int p1[2], p2[2], p3[2], p4[2];
    char buf[10];
    pid_t cpid1, cpid2;
    pipe(p1);
    pipe(p2);
    pipe(p3);
    pipe(p4);
    if((cpid1 = fork()) == 0){
        sprintf(buf, "%s", "Delhi\n");
        close(p2[0]);
        write(p2[1], buf, strlen(buf));
        close(p1[1]);
        read(p1[0], buf, 10);
        printf("String read from Parent by C1 : %s", buf);
        sprintf(buf, "%s", "Chennai\n");
        close(p4[0]);
        write(p4[1], buf, strlen(buf));
    }
    else if((cpid2 = fork()) == 0){
        sprintf(buf, "%s", "Cochin\n");
        close(p3[0]);
        write(p3[1], buf, strlen(buf));
        close(p4[1]);
        read(p4[0], buf, 10);
        printf("String read from C1 by C2 : %s", buf);
    
    }
    else {
        close(p2[1]);
        read(p2[0], buf, 10);
        printf("String read from Child1 : %s", buf);
        sprintf(buf, "%s", "Mumbai\n");
        close(p1[0]);
        write(p1[1], buf, strlen(buf));
        close(p3[1]);
        read(p3[0], buf, 10);
        printf("String read from C2 : %s", buf);
    }
    return 0;
}
        
        /*mumbai, p-c1*/
