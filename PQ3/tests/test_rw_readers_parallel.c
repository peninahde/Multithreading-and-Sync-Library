/*
- Create 8 reader threads that all acquire the read lock, then stay in the read section for a short time (e.g., a
loop), then release.
- Verify (using an atomic counter) that at some point there were at least 2 readers inside simultaneously
- Verify no deadlock

return 0 on success
return 1 on failure
(Printing a short failure message is recommended.)
*/ 

#include <stdio.h>
#include <stdatomic.h>
#include <sched.h>
#include <pthread.h>
#include <assert.h>
#include "../rw_lock.h"

#define N 8

atomic_int test_active_readers = 0;
atomic_int test_max_seen_readers = 0;

void* reader_thread_func(void* args){

    rwlock* lock = (rwlock*)args;

    rwlock_acquire_read(lock);
    
    int current = atomic_fetch_add(&test_active_readers, 1) + 1;

    int seen = atomic_load(&test_max_seen_readers);
    while(current > seen && !atomic_compare_exchange_weak(&test_max_seen_readers, &seen, current));

    for (volatile int i = 0; i < 1000000; i++);
    
    atomic_fetch_sub(&test_active_readers, 1);

    rwlock_release_read(lock);
    return NULL;
}

int main(){
    pthread_t readers[N];
    rwlock lock;

    printf("------ Starting Readers Parallel Test ------\n");

    //initialize the lock
    rwlock_init(&lock); 
    atomic_init(&test_active_readers, 0);
    atomic_init(&test_max_seen_readers, 0);

    //create 8 readers
    for(int i = 0; i < N; i++)
    {
        if(pthread_create(&readers[i], NULL, reader_thread_func, &lock) != 0){
            perror("Failed to create reader thread");
            return 1;
        }
    }
     //join the threads
    for(int i = 0; i < N; i++)
    {
        pthread_join(readers[i], NULL);
    }

    int result = atomic_load(&test_max_seen_readers);
    printf("Max readers seen together: %d\n", result);

    if (result >= 2){
        printf("----SUCCESS: Parallelism detected babyyyy! ----\n");
    } else {
        printf("----FAILED: Parallelism not detected :( ----\n");
        return 1;
    }
    return 0;
}
