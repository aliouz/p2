 # __Project 2 - User-level thread library__

## Phase 1 - In this phase we used a linked list to implement a queue.
We chose a linked list implementation because it would allow us to enqueue and
dequeue with a complexity of O(1) and iterating through the queue would have a
complexity of O(n). This is because enqueuing and dequeuing require inserting a
node at the end of the queue or deleting a node at the beginning, which are both
O(1) operations for linked lists.

## Phase 2 - In this phase we implemented part of the thread library
### uthread_block();
The requirement of this method was for the calling thread to be blocked and not
be ready to run until it was unblocked. The method also had to allow for the
next ready thread to run. To accomplish this we needed to repeat the steps of
yield, except instead of storing the current running thread in the ready queue,
we only changed its state and put the next thread from the ready queue on the
CPU. We did this because the thread should not be ready to run until it is
unblocked. Since this method accessed the ready queue which was shared data, we
put the parts of this method that accessed the queue in a preempt disable and
enable block.

## Phase 2 - uthread:
#### uthread_yield();
The requirement of this method was for the thread calling it to give up control
of the CPU and allow another ready thread to run. To accomplish this, we needed
to store the currently running thread back in the ready queue and put the first
thread in the ready queue on the CPU. We did this by first dequeuing from the
ready queue and storing that thread in a temporary variable. We then enqueued
the current thread. Before dequeuing we needed to check if the queue was empty,
as yielding shouldn’t remove the current thread from the CPU if there is no
other thread waiting to use it. To actually make the CPU run the thread we
dequeued, we need to call uthread_ctx_switch(). Within this method we accessed
shared data, such as the pointer to the current running thread as well as the
queue, so we disabled interrupts before this code and enabled after.

#### uthread_create();
The requirement of this method was to create a thread that a function
with the arguments that were passed to it. We started by initialzing a thread using 
our inttilize_thread(); function. This function just allocates space for all the 
members in the thread struct. We then have to Initialize the execution content of 
the thread using uthread_ctx_init(). This sets up the content in the stack and 
context members of our struct. This can fail, so we have to check if the execution 
content is initialized correctly, if not, we return -1. Finally we put the thread 
into out ready queue.

### uthread_ready();
The uthread_run function is start of our multithreading library. As this is the 
first function that will be called to start using thread, we need to initialzie
everything we will need. We initialize our idle thread called current_thread and
two queues that will hold threads in different states, ready_queue and zombie_queue.
We then create the first thread to dun the passed function and arguments. Until all
of the threads have ended their execution, we yield as to let every thread run. If
preemtion wanted, we start it at the the beggining of this method and stop it at
the end as after all the threads are done running, preemption is no longer needed.

### uthread_block();
The requirement of this method was for the calling thread to be blocked and not
be ready to run until it was unblocked. The method also had to allow for the
next ready thread to run. To accomplish this we needed to repeat the steps of
yield, except instead of storing the current running thread in the ready queue,
we only changed its state and put the next thread from the ready queue on the
CPU. We did this because the thread should not be ready to run until it is
unblocked. Since this method accessed the ready queue which was shared data, we
put the parts of this method that accessed the queue in a preempt disable and
enable block.

### uthread_unblock();
The requirement of this method was for the calling thread to become unblocked
and be ready to run. We did this by changing the state of the thread to ready
and enqueuing it in the ready queue.

## Phase 3 - Semaphores:

Our semaphore implemenation is based around the semaphores struct
if contains two members:
 - resource_count, an unsigned int that keeps track of the resources
 - thread_wait, a queue that keeps track of threads waiting for a resource

#### sem_create();
In this function we just initialize the struct and its members. If the 
semaphore is null we return NUll and if not, we return the semaphore

#### sem_destroy();
This metthod is meant to wipe the semaphore and deallocate its space. To
do this we have to first check if the semaphore is null or if there are threads 
still waiting for a resource. If this is the case we must return an error. If 
this is not the case then we can destroy the thread_qait queue and call free on
the semphare to comepleteley deallocate its space.

#### sem_down();
The goal of sem_down was to decrement the resource count of the given semaphore
or block the thread if resources weren’t available. We did this by first
checking if the semaphore resource count was greater than 0 and if so we
decremented the count and returned. However, if the count was 0, we enqueued the
thread in our waiting queue and blocked it, so that it would not have access to
the resource until it was unblocked (which would occur when the resource count
was incremented). We decided to put everything from the check of the resource
count to the decrementing of the count in a preempt disable and enable block,
because if another thread were to release the resource after we checked the
count but before we blocked the current thread, the thread would be blocked
even though a resource is available. Therefore those steps needed to be done
atomically.

#### sem_up();
The goal of sem_up was to increment the resource count of the given semaphore
and allow the next waiting thread (if any) to use the semaphore. We did this by
first increasing the resource count and then unlocking the next thread in the
waiting queue. We decided to put everything from the incrementation of the
resource count to the unlocking of the thread in a preempt disable and enable
block, because if another thread were to use the semaphore after we incremented
the resource count but before we unblocked the next waiting thread, the thread
would attempt to use a resource that isn’t available. Therefore those steps
needed to be done atomically.

## Phase 4 - Preempt:

#### sig_handler();
This functions main purpose it to tell the handler what to do when a 
specific singal happens in our case, it SIGVTALRM. When that signal
happens this function is called and calls thread_yield();

#### preempt_enable()
This method is used after preempt_disable() is called so that threads can
continue to be interrupted by the alarm. We did this by unblocking the signal
that caused the interruption to occur.

#### preempt_disable()
The goal of this method was to disable preemption so that the thread would
continue to execute without interruption. We did this by blocking the signal
that caused the interruption to occur.

#### preempt_start()
This function is supposed to send a signal of SIGVTALRM every 10 milliseconds
so that we can yield to the next thread. To do this we are create a sigaction
struct called sa that when a SIGVTALRM signal happens, it calls our sig_hanler()
function. To get the SIGVTALRM signal to go off every 10 milliseconds, we have 
to set up a timer. We create a timer variable using the itimerval struct and 
initializes its members so that it runs every 10 milliseconds. We then have to
set the timer to send the SIGVTALRM signal when it is done, so we use the
ITIMER_VIRTUAL macro to do this. After the fucntion finishes, the time starts 
and every 10 milliseconds the SIGVTALRM signal is called, calling out sig_handler
function and yielding the thread.

#### preempt_stop()
This method disables the preempt and sets the timer to null so that the signal
handler will no longer get any SIGVTALRM signals.

#### test_preempt()
We test preemption by creating 2 threads, documenting when they are running, and
checking if the CPU is switching between them. We have both threads execute a
for loop that runs 2000000000 times and prints something every 200000000 loops.
This tests preemption because we can see if the two threads are executing
concurrently if their print statements occur within each other. If all the print
statements of one thread occur before the other, it means the threads were not
being preempted, as their execution was all in one go.
