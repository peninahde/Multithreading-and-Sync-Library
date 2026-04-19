// create 8 threads that all call semaphore_wait() once; the main
// thread calls semaphore_signal() exactly 8 times; verify all 8 threads finish.
#include <stdio.h>
#include <stdatomic.h>
#include <sched.h>
#include <pthread.h>
#include "../tl_semaphore.h"

#define N 8

void* thread_func(void* args)
{
    semaphore* sem = (semaphore*)args;

    semaphore_wait(sem);
    printf("Thread passed the semaphore.\n");

    return NULL;
}


int main()
{
    pthread_t threads[N];
    semaphore sem;

    printf("------ Starting Concurrent Test ------\n");

    //initialize the semaphore
    semaphore_init(&sem, 0);

    // create 8 threads
    for(int i = 0; i < N; i++)
    {
        pthread_create(&threads[i], NULL, thread_func, &sem);
    }

    printf("All threads have been successfully created! Staring to test signaling...\n");

    for(int i = 0; i < N; i++)
    {
        semaphore_signal(&sem);
    }

    //join the threads
    for(int i = 0; i < N; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("---- SUCCESS: All threads have finishedddd. ----\n");
    return 0;
}