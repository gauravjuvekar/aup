#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
int main() {
    int fr = open("file1", O_RDONLY);
    int fw = open("file2", O_WRONLY | O_CREAT, 0777);
	close(0);
	close(1);
    dup2(fr, 0);    /*Redirects stdin to fr*/
    dup2(fw, 1);    /*Redirects stdout to fw*/
    char str[20];
    scanf("%[^\n]s", str);
    printf("%s\n", str);
    return 0;
}
