#include<stdio.h>
#include<pthread.h>
void *getId(void *arg){
	int id = (long int)arg;
	printf("Thread id :- %d\n", id);	
	return;
}

int main(){

	long int i, n = 5;
	pthread_t threadn[n];
	for(i = 0; i < n; i++){
		pthread_create(&threadn[i], NULL, getId, (void*)i);
		pthread_join(threadn[i], NULL);
	}
	return;
}
