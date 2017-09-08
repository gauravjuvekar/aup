#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
int main() {
    int fr = open("file1.txt", O_RDONLY);
    int fw = open("file2.txt", O_WRONLY | O_CREAT, 0777);
    dup2(fr, 0);    /*Redirects stdin to fr*/
    dup2(fw, 1);    /*Redirects stdout to fw*/
    char str[20];
    scanf("%[^\n]s", str);
    printf("%s\n", str);
    return 0;
}
