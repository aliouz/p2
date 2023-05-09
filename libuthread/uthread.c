#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"
#include "queue.h"

enum threadState {Ready, Running, Blocked, Zombie};

queue_t ready_queue;
queue_t zombie_queue;

typedef struct uthread_tcb {
	void *stack;
	int state;
	uthread_ctx_t *context; 
}thread;

thread *current_thread;

thread *uthread_current(void)
{
    return current_thread;
}

thread *intialize_thread(int state){
    thread *curr_thread = malloc(sizeof(thread));
    curr_thread->stack = uthread_ctx_alloc_stack();
  
	curr_thread->state = state;
	curr_thread->context = malloc(sizeof(uthread_ctx_t));
    return curr_thread;
}

void uthread_yield(void)
{
    if (queue_length(ready_queue) != 0) {
        thread *next_thread;
        queue_dequeue(ready_queue, (void**)&next_thread);
        queue_enqueue(ready_queue, current_thread);
        uthread_ctx_switch(current_thread->context, next_thread->context);
        current_thread = next_thread;
    }
    /* TODO Phase 2 */
}

void uthread_exit(void)
{
    current_thread->state = Zombie;
    queue_enqueue(zombie_queue, current_thread);
    uthread_ctx_destroy_stack(current_thread->stack);
    queue_dequeue(ready_queue, (void**)&current_thread);

}

int uthread_create(uthread_func_t func, void *arg)
{
    //intialize the thread
	thread *curr_thread = intialize_thread(Ready);
    int status = uthread_ctx_init(curr_thread->context, curr_thread->stack, func, arg);

    //checks if there is a context error
    if(status == -1){
        return -1;
    }

    queue_enqueue(ready_queue,curr_thread);
    return 0;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
    if(preempt){
        printf("yo");
    }
    
    current_thread = intialize_thread(Running);
    ready_queue = queue_create();
    zombie_queue = queue_create();

    int status = uthread_create(func,arg);
    if(ready_queue == NULL || status == -1 ){
        return -1;
    }

    while(queue_length(ready_queue)){
        uthread_yield();
    }

    return 0;
}

