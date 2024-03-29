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

queue_t ready_queue; // queue for storing threads ready to run
queue_t zombie_queue; //queue for storing threads in zombie state
bool isPreempt; // global flag for checking if preempts are enabled

typedef struct uthread_tcb {
	void *stack;
	int state;
	uthread_ctx_t *context; 
}thread;

thread *current_thread; // global pointer for current thread running

thread *uthread_current(void)
{
    return current_thread;
}

//this function is used to intitalze a thread and its memebers
thread *intialize_thread(int state){
    preempt_disable();
    thread *curr_thread = malloc(sizeof(thread));
    curr_thread->stack = uthread_ctx_alloc_stack();
	curr_thread->state = state;
	curr_thread->context = malloc(sizeof(uthread_ctx_t));
    preempt_enable();
    return curr_thread;
}

// yields thread and runs next thread in ready queue 
void uthread_yield(void)
{
    preempt_disable();
    if (queue_length(ready_queue) != 0) {
        thread *next_thread;
        // put current running thread in queue and dequeue the next ready thread
        queue_dequeue(ready_queue, (void**)&next_thread);
        queue_enqueue(ready_queue, current_thread);
        thread *temp_thread = current_thread;
        current_thread = next_thread;
        preempt_enable();
        uthread_ctx_switch(temp_thread->context, current_thread->context);   
    } else {
        preempt_enable();
    }
}

// function run when thread is done executing
void uthread_exit(void)
{
    preempt_disable();
    current_thread->state = Zombie;
    queue_enqueue(zombie_queue, current_thread);
    uthread_ctx_destroy_stack(current_thread->stack);
     
    if (queue_length(ready_queue) != 0) {
        thread *next_thread = intialize_thread(Zombie);
        queue_dequeue(ready_queue, (void**)&next_thread);
        thread *temp_thread = current_thread;
        current_thread = next_thread;
        preempt_enable();
        uthread_ctx_switch(temp_thread->context, current_thread->context);
    } else {
        preempt_enable();
    }
}

// creates thread context
int uthread_create(uthread_func_t func, void *arg)
{
    preempt_disable();
    //intialize the thread
	thread *curr_thread = intialize_thread(Ready);
    int status = uthread_ctx_init(curr_thread->context, curr_thread->stack, func, arg);
    //checks if there is a context error
    if(status == -1){
        return -1;
    }
    queue_enqueue(ready_queue,curr_thread);
    preempt_enable();
    return 0;
}

// runs threads from ready queue 
int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
    //if preempt is false it will return
    //else it starts preemption
    preempt_start(preempt);

    //initialize the current thread and queues
    current_thread = intialize_thread(Running);
    ready_queue = queue_create();
    zombie_queue = queue_create();

    int status = uthread_create(func,arg);

      
    //if ready queue is null or status is anything other than 0 return
    if(!ready_queue || status){
        return -1;
    }

    //goes through ready queue and yields each thread
    while(queue_length(ready_queue)){
        uthread_yield();
    }

    //once threads are done running preemption stops
    preempt_stop();
    return 0;
}

// blocks current thread running
void uthread_block(void)
{
    preempt_disable();
    current_thread->state = Blocked;
    // if ready queue has threads 
    if (queue_length(ready_queue) != 0) {
        thread *next_thread;
        queue_dequeue(ready_queue, (void**)&next_thread);
        // switch context to next thread ready to run
        thread *temp_thread = current_thread;
        current_thread = next_thread;
        preempt_enable();
        uthread_ctx_switch(temp_thread->context, current_thread->context);
    } else {
        preempt_enable();
    }
}

// unblocks passed in queue
void uthread_unblock(struct uthread_tcb *uthread)
{
	/* TODO Phase 3 */
    uthread->state = Ready;
    preempt_disable();
    queue_enqueue(ready_queue, uthread); // put thread back in ready queue 
    preempt_enable();
}