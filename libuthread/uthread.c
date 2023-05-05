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



 
// Data structure to represent a stack
struct stack
{
    int maxsize;    // define max capacity of the stack
    int top;
    int *items;
};
 
// Utility function to initialize the stack
struct stack* newStack(int capacity)
{
    struct stack *pt = (struct stack*)malloc(sizeof(struct stack));
 
    pt->maxsize = capacity;
    pt->top = -1;
    pt->items = (int*)malloc(sizeof(int) * capacity);
 
    return pt;
}
 
// Utility function to return the size of the stack
int size(struct stack *pt) {
    return pt->top + 1;
}
 
// Utility function to check if the stack is empty or not
int isEmpty(struct stack *pt) {
    return pt->top == -1;                   // or return size(pt) == 0;
}
 
// Utility function to check if the stack is full or not
int isFull(struct stack *pt) {
    return pt->top == pt->maxsize - 1;      // or return size(pt) == pt->maxsize;
}
 
// Utility function to add an element `x` to the stack
void push(struct stack *pt, int x)
{
    // check if the stack is already full. Then inserting an element would
    // lead to stack overflow
    if (isFull(pt))
    {
        printf("Overflow\nProgram Terminated\n");
        exit(EXIT_FAILURE);
    }
 
    printf("Inserting %d\n", x);
 
    // add an element and increment the top's index
    pt->items[++pt->top] = x;
}
 
// Utility function to return the top element of the stack
int peek(struct stack *pt)
{
    // check for an empty stack
    if (!isEmpty(pt)) {
        return pt->items[pt->top];
    }
    else {
        exit(EXIT_FAILURE);
    }
}
 
// Utility function to pop a top element from the stack
int pop(struct stack *pt)
{
    // check for stack underflow
    if (isEmpty(pt))
    {
        printf("Underflow\nProgram Terminated\n");
        exit(EXIT_FAILURE);
    }
 
    printf("Removing %d\n", peek(pt));
 
    // decrement stack size by 1 and (optionally) return the popped element
    return pt->items[pt->top--];
}




struct uthread_tcb {
	/* TODO Phase 2 */
	struct stack threadStack;
	enum threadState {Ready, Running, Blocked, Zombie};
	int cpuRegisters[]; 
};

struct uthread_tcb *uthread_current(void)
{
	/* TODO Phase 2/3 */
}

void uthread_yield(void)
{
	/* TODO Phase 2 */
}

void uthread_exit(void)
{
	/* TODO Phase 2 */
}

int uthread_create(uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
}

void uthread_block(void)
{
	/* TODO Phase 3 */
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	/* TODO Phase 3 */
}

