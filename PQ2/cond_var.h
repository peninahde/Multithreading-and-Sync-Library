#include "tl_semaphore.h"
#include <stdatomic.h>
#include <pthread.h>

#define MAX_THREADS 100

typedef struct condition_variable {
        pthread_t* waiting_threads;
        atomic_int current_end;
        atomic_int current_thread;
        atomic_int current_gen;
} condition_variable;


/*
* Initializes the condition variable pointed to by 'cv'.
*/
void condition_variable_init(condition_variable* cv);

/*
* Precondition: the calling thread holds 'ext_lock'.
*
* Causes the calling thread to wait on the condition variable 'cv'.
* While waiting, the thread must release 'ext_lock' and must reacquire it before
returning.
*
* IMPORTANT correctness rules (no "lost notifications"):
*
* - Signals are NOT remembered:
* If condition_variable_signal/broadcast is called when no threads are waiting,
* it has no effect.
*
* - A waiting thread must not get "stuck" due to a race:
* If a thread T is inside condition_variable_wait (it has started the wait operation
* and has not returned yet), and another thread later calls
* condition_variable_signal/broadcast, then T must eventually wake and return
* (after reacquiring ext_lock).
*/
void condition_variable_wait(condition_variable* cv, ticket_lock* ext_lock);

/*
* Wakes up exactly one thread currently waiting on the condition variable 'cv' (if any).
* If no threads are waiting, this function does nothing.
*/
void condition_variable_signal(condition_variable* cv);

/*
* Wakes up all threads currently waiting on the condition variable 'cv'.
* If no threads are waiting, this function does nothing.
*/
void condition_variable_broadcast(condition_variable* cv);
