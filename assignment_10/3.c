#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>

#define SIZE sizeof(int);


void sem_incr(int semid, int sem_index) {
	struct sembuf sem_op;
	sem_op.sem_flg = 0;
	sem_op.sem_num = sem_index;
	sem_op.sem_op = 1;

	if (semop(semid, &sem_op, 1) < 0) {
		perror("semop");
	}
}


void sem_decr(int semid, int sem_index) {
	struct sembuf sem_op;
	sem_op.sem_flg = 0;
	sem_op.sem_num = sem_index;
	sem_op.sem_op = -1;

	if (semop(semid, &sem_op, 1) < 0) {
		perror("semop");
	}
}

int main() {
	int forkval = fork();
	if (forkval < 0) {
		perror("fork");
		return 1;
	}

	key_t sem_key = ftok(".", 12);
	key_t mem_key = ftok(".", 20);

	if ((sem_key == -1) || (mem_key == -1)) {
		perror("ftok");
		return 1;
	}

	int semid = semget(sem_key, 2, IPC_CREAT | 0600);

	if (semid == -1) {
		perror("semget");
		return 1;
	}

	int shmid = shmget(mem_key, 4, IPC_CREAT | 0600);

	if (shmid == -1) {
		perror("shmget");
		return 1;
	}

	volatile void *mem = shmat(shmid, NULL, 0);
	if (mem == (void *)-1) {
		perror("shmget");
		return 1;
	}

	int i;
	if (forkval) {
		/* Parent - producer */

		/* Initialize the semaphores */
		unsigned short initial_val[2] = {0, 0};
		if (semctl(semid, 0, SETALL, &initial_val[0]) < 0) {
			perror("semctl(SETALL)");
		}

		sem_incr(semid, 1);
		for (i = 10; i >= 0; i--) {
			sem_decr(semid, 1);
			*((int *)mem) = i;
			__sync_synchronize();
			sem_incr(semid, 0);
		}
	}
	else {
		/* Child - consumer */

		/* Wait til parent initializes the semaphores */
		struct semid_ds initial_semaphore;
		do {
			if (semctl(semid, 0, IPC_STAT, &initial_semaphore) < 0) {
				perror("semctl(IPC_STAT)");
			}
		} while (!initial_semaphore.sem_otime);

		do {
			sem_decr(semid, 0);
			__sync_synchronize();
			i = *((int *)mem);
			sem_incr(semid, 1);
			printf("%d\n", i);
		} while (i);
		if (shmctl(shmid, IPC_RMID, NULL) < 0) {
			perror("shmctl(IPC_RMID)");
		}
		if (semctl(semid, 0, IPC_RMID) < 0) {
			perror("semctl(IPC_RMID)");
		}
	}
}
