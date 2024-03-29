#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
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
    sigemptyset(&ss);
    sigaddset(&ss, SIGVTALRM);
    sigprocmask(SIG_BLOCK, &ss, NULL); // blocks alarm signal
}

void preempt_enable(void)
{
    sigemptyset(&ss);
    sigaddset(&ss, SIGVTALRM);
    sigprocmask(SIG_UNBLOCK, &ss, NULL); // unblocks alarm signal
}

void preempt_start(bool preempt)
{
	//sets global variable so ther functions can access preempt bool
	preempt_status = preempt;
	if(!preempt){
		return;
	}
	//initialize struct sigaction sa;
	sa.sa_handler = sig_handler;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGVTALRM);
	sa.sa_flags = 0;
	sigaction(SIGVTALRM, &sa, NULL);

	//initialize and set timer
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
		//resets timer
		setitimer(ITIMER_VIRTUAL, &it, NULL);
		preempt_status = false;
	}
}

