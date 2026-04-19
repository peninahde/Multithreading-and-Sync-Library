// create 16 threads and run many operations (e.g., each thread performs at least 10,000 wait / signal 
// operations in a loop); verify no deadlock and that the final semaphore value is exactly
// what you expect.
#include <stdio.h>
#include <stdatomic.h>
#include <assert.h>
#include <sched.h>
#include <pthread.h>
#include "../tl_semaphore.h"

#define N 16
#define I 10000

void* thread_func(void* args)
{
    semaphore* sem = (semaphore*)args;

    for (int i = 0; i < I; i++)
    {
        semaphore_wait(sem);
        semaphore_signal(sem);
    }
    return NULL;
}

int main()
{
    pthread_t threads[N];
    semaphore sem;

    printf("------ Starting Stress Test (get ready!!!) ------\n");

    //initialize the semaphore
    semaphore_init(&sem, 1);

    // create 16 threads
    for(int i = 0; i < N; i++)
    {
        pthread_create(&threads[i], NULL, thread_func, &sem);
    }

    printf("All threads have been successfully created and are all signaling...\n");

    //join the threads after the 10,000 operations
    for(int i = 0; i < N; i++)
    {
        pthread_join(threads[i], NULL);
        //a check to see which get stuck
        printf("Thread %d successfully returned. \n", i);
    }

    //check that they all actually finished and we got back to the original value
    printf("Final semaphore value: %d\n", sem.value);
    assert(sem.value ==1);

    printf("---- SUCCESS: All threads have finishedddd. ----\n");
    return 0;
}