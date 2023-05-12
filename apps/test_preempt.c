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

    // printf("Without preempts\n");
    // uthread_run(false, thread1, NULL);
    return 0;
}













// #include <stdio.h>
// #include <stdlib.h>
// #include <uthread.h>

// void busy_loop(void) {
//     int i = 0;
//     while (i < 1000000000) {
//         i++;

//         if (i % 100000000 == 0) {
//             printf("Busy loop %d\n", i / 100000000);
//         }
//     }
// }

// void thread_func1(void *arg) {
//     (void)arg;

//     printf("Thread 1 starting...\n");
//     for (int i = 0; i < 5; i++) {
//         printf("Thread 1 running...\n");
//         uthread_yield();
//     }
//     printf("Thread 1 done.\n");
// }

// void thread_func2(void *arg) {
//     (void)arg;

//     uthread_create(thread_func1, NULL);

//     printf("Thread 2 starting...\n");
//     busy_loop();
//     printf("Thread 2 done.\n");
// }

// int main(void) {
//     uthread_run(true, thread_func2, NULL);

//     printf("Test complete.\n");

//     return 0;
// }


/*
void busy_loop(void) {
    int i = 0;
    while (i < 1000000000) {
        i++;

        if (i % 100000000 == 0) {
            printf("\tThread 2 run #%d\n", i);
        }
    }
}

void thread_func1(void *arg) {
    (void)arg;

    printf("=== Thread 1 starting ===\n");
    for (int i = 0; i < 20; i++) {
        printf("Thread 1 run #%d\n", i);
        uthread_yield();
    }
    printf("=== Thread 1 done ===\n");
}

void thread_func2(void *arg) {
    (void)arg;
    //preempt_disable();
    uthread_create(thread_func1, NULL);

    printf("=== Thread 2 starting ===\n");
    busy_loop();
    printf("=== Thread 2 done ===\n");
}

int main(void) {
    uthread_run(true, thread_func2, NULL);
    
    printf("Test complete.\n");

    return 0;
}
*/