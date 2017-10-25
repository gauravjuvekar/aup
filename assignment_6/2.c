#define _GNU_SOURCE
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<sched.h>
#include<sys/sysinfo.h>
void *attach_thr(void *);
int main() {
    int nprocs = get_nprocs_conf(); /*Get number of processors*/
    int i, arg[nprocs];
    pthread_t thread_id[nprocs];
    
    printf("Number of CPUs : %d\n\n", nprocs);
    
    for(i = 0; i < nprocs; i++){
        arg[i] = i;
        pthread_create(&thread_id[i], NULL, attach_thr, &arg[i]);
    }
    
    for(i = 0; i < nprocs; i++)
        pthread_join(thread_id[i], NULL);
    
    return 0;
}
void *attach_thr(void *proc){
    int cpu_no = *(int *)proc;
    
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);  /*Clear cpuset */
    CPU_SET(cpu_no, &cpuset); /*Set bit in 'cpuset', corresponding to 'cpu_no'*/

    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    pthread_getaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);

    if(CPU_ISSET(cpu_no, &cpuset))
        printf("Thread attached successfully to CPU %d\n", cpu_no);
    else
        printf("Thread did not attach to CPU %d\n", cpu_no);
}
