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
	semaphore *sem = malloc(sizeof(semaphore));
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
	if (sem == NULL) {
		return -1;
	}
	while (sem->resource_count < 1) {
		queue_enqueue(sem->thread_wait, uthread_current());
		uthread_block();
	}
	preempt_disable();
	sem->resource_count -= 1;
	preempt_enable();
	return 0;
	
}

int sem_up(sem_t sem)
{
	if (sem == NULL) {
		return -1;
	}
	preempt_disable();
	sem->resource_count += 1;
	preempt_enable();
	if(queue_length(sem->thread_wait) > 0) {
		struct uthread_tcb *uthread = NULL;
		queue_dequeue(sem->thread_wait, (void**)&uthread);
		uthread_unblock(uthread);
	}
	return 0;
	/* TODO Phase 3 */
}

