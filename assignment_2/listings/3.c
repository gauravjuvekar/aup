#include<stdio.h>
#include<dirent.h>
#include<stdlib.h>
int main(int argc, char *argv[]){
        if(argc > 2){
                fprintf(stderr,"Usage : ./create_dir <dir_name>\n");
                exit(1);
        }
        mkdir(argv[1], 777);
        chmod(argv[1], 01777);
}
