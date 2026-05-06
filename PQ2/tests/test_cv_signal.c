#include "tests.h"

#define N 8

volatile int counter = 0;
volatile atomic_int waiting = 0;

void* incCounter(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    condition_variable* cv = args->cv;
    ticket_lock* ext = args->ext;
    ticketlock_acquire(ext);
    atomic_fetch_add(&waiting, 1);
    condition_variable_wait(cv, ext);
    counter++;
    ticketlock_release(ext);
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

    while (atomic_load(&waiting) < N) { //Otherwise It might signal before it starts to wait
        sched_yield();
    }

    for (int i = 0; i < N; i++) {
        condition_variable_signal(cv);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    if(counter != N) {
        printf("Error! expected %d but received %d\n", N, counter);
        return 1;
    }

    return 0;
}