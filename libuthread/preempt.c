#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

struct itimerval it_new, it;
struct sigaction sa;
sigset_t ss;
bool preempt_status;

void sig_handler(int signum){
	if(signum == SIGVTALRM){
		uthread_yield();
	}
}

void preempt_disable(void)
{
    /* TODO Phase 4 */
    sigset_t ss;
    sigemptyset(&ss);
    sigaddset(&ss, SIGVTALRM);
    pthread_sigmask(SIG_UNBLOCK, &ss, NULL);
}

void preempt_enable(void)
{
    /* TODO Phase 4 */
    sigset_t ss;
    sigemptyset(&ss);
    sigaddset(&ss, SIGVTALRM);
    pthread_sigmask(SIG_BLOCK, &ss, NULL);
}

void preempt_start(bool preempt)
{
	preempt_status = preempt;
	if(!preempt){
		return;
	}

	sa.sig_handler = &sig_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGVTALRM, &sa.sa_mask, NULL);

	it_new.it_interval.tv_sec = 0;
	it_new.it_interval.tv_usec = 100 * HZ;
	it_new.it_value.tv_sec = 0;
	it_new.it_value.tv_usec = 100 * HZ;
	setitimer(ITIMER_VIRTUAL, &it_new, &it);
	
}

void preempt_stop(void)
{
	if(preempt_status){
		preempt_disable();
		setitimer(ITIMER_VIRTUAL, &it, NULL);
		sigaction(SIGVTALRM, &sa, NULL)
		preempt_status = false;
	}
}

