/* Values in memory are left alone
 * Values in registers are rolled back
 */
#include<stdio.h>
#include<setjmp.h>
#include<stdlib.h>
jmp_buf buffer;
void f1();
void f2();
int glob = 4;
int main() {
    int ret;
    auto int aut = 1;
    volatile int vol = 2;
    register int reg = 3;
    static int stat = 5;
    if((ret = setjmp(buffer)) != 0){
        /* 'ret' will be equal to second argument of longjmp*/
        printf("After Long jump, Return val%d\n", ret);
        printf("Aut : %d, Vol : %d, Reg : %d, Glob : %d, Stat : %d\n\n", aut, vol, reg, glob, stat);
        exit(0);
    }
    else {
        printf("Direct call, Return val: %d\n", ret);
        printf("Aut : %d, Vol : %d, Reg : %d, Glob : %d, Stat : %d\n\n", aut, vol, reg, glob, stat);
    }
    aut = 10;
    vol = 20;
    reg = 30;
    glob = 40;
    stat = 50;
    f1(aut, vol, reg, stat);
    return 0;
}
void f1(int i, int j, int k, int l){
    printf("In f1() : \nAuto = %d,Vol  = %d, Reg = %d,Glob  = %d,Stat = %d\n\n", i, j, k, glob, l);
    f2();
}
void f2(){
    longjmp(buffer, 1);     
}
