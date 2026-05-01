#include "tests.h"

#define N 16
#define LOOPS 1000

volatile int counter = 0;
volatile atomic_int waiting = 0;

void* incCounter(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    condition_variable* cv = args->cv;
    ticket_lock* ext = args->ext;
    for (int i = 0; i < LOOPS; i++) {
        ticketlock_acquire(ext);
        atomic_fetch_add(&waiting, 1);
        condition_variable_wait(cv, ext);
        counter++;
        ticketlock_release(ext);
    }
    return 0;
}

int main() {
    signal(SIGSEGV, handler);
    condition_variable* cv = malloc(sizeof(condition_variable));
    ticket_lock* tck = malloc(sizeof(ticket_lock));
    condition_variable_init(cv);
    ticketlock_init(tck);
    ThreadArgs* args = malloc(sizeof(ThreadArgs));
    args->cv = cv;
    args->ext = tck;
    pthread_t* threads = malloc(sizeof(pthread_t) * N);
    for (int i = 0; i < N; i++) {
        pthread_create(&threads[i], NULL, incCounter, args);
    }
    for (int i =0; i < LOOPS; i++) {
        while (atomic_load(&waiting) < N) {
            sched_yield();
        }
        ticketlock_acquire(tck); //Otherwise there might be a context switch before the thread is waiting
        ticketlock_release(tck); //resulting in broadcasting before everyone is waiting
        atomic_store(&waiting, 0);
        condition_variable_broadcast(cv);
    }
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }
    if(counter != N*LOOPS) {
        printf("Error! expected %d but received %d\n", N, counter);
        return 1;
    }
    return 0;
}