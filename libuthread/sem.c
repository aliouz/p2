#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"
#include "uthread.h"

typedef struct semaphore {
	unsigned int resource_count;
	queue_t thread_wait;
}semaphore;

sem_t sem_create(size_t count)
{
	semaphore sem = malloc(sizeof(semaphore));
	if(!sem){
		return NULL;
	}

	sem->resource_count = count;
	sem->thread_wait = queue_create();

	return sem;

}

int sem_destroy(sem_t sem)
{
	if(!sem || sem->thread_wait){
		return -1;
	}

	queue_destroy(sem->thread_wait);
	free(sem);
	return 0;

}

int sem_down(sem_t sem)
{
	/* TODO Phase 3 */
}

int sem_up(sem_t sem)
{
	/* TODO Phase 3 */
}

