#include <stdio.h>
#include <stdlib.h>
#include <uthread.h>
#include <preempt.c>


#define MAXCOUNT 20


static void thread2()
{
    for(int x = 0; x < 2000000000; x++) {
        if (x % 200000000 == 0) {
            printf("Thread 2 run number: %d\n", x);
        }
    }
}

static void thread1()
{
    uthread_create(thread2,NULL);
    for(int x = 0; x < 2000000000; x++) {
        if (x % 200000000 == 0) {
            printf("Thread 1 run number: %d\n", x);
        }
    }

}
int main()
{

    printf("With preempts\n");
    uthread_run(true, thread1, NULL);

    return 0;
}
