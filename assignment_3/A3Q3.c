#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
int main() {
        mode_t curr_mask;
        curr_mask = umask(0); /*set umask to random value*/
        umask(curr_mask); /*restore previous umask*/
        return 0;
}        
