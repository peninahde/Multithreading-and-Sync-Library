/*
- Create 12 reader threads that repeatedly: acquire_read → short work → release_read.
- Create 4 writer threads that repeatedly: acquire_write → short work → release_write.
- Run for enough iterations (e.g., 5,000 total acquisitions across writers).
- Verify every writer thread successfully acquires the write lock many times (e.g., at least 100 acquisitions
each), i.e., writers do not starve.

return 0 on success
return 1 on failure
(Printing a short failure message is recommended.)
*/

#include <stdio.h>
#include <stdatomic.h>
#include <sched.h>
#include <pthread.h>
#include "../rw_lock.h"

#define N1 12
#define N2 4

typedef struct {
    rwlock* lock;
    int id;
} thread_args;

static atomic_int writer_success[4] = {0,0,0,0};
//static atomic_int readers_in_cs = 0;       
//static atomic_int writer_blocked_by_reader = 0;

void* writer_thread_func(void* args){

    thread_args* t_args = (thread_args*)args;
    rwlock* lock = t_args->lock;
    int id = t_args->id;
    
    for (int i = 0; i < 1250; i++){

        //FOR TESTING PURPOSES: see if the readers are actually blocking the writers
        //if (atomic_load(&readers_in_cs) > 0) {
        //    atomic_fetch_add(&writer_blocked_by_reader, 1);
        //}
        
        rwlock_acquire_write(lock);
        //increase counter for specific writer that acquires lock
        atomic_fetch_add(&writer_success[id], 1);

        // Diagnostic Print
        //printf("[WRITER %d] Inside critical section (Acquisition #%d)\n", id, i + 1);

        //wait long enough to catch any readers
        for(volatile int j = 0; j < 50000; j++);

        rwlock_release_write(lock);

        sched_yield();

    }

    return NULL;
}

void* reader_thread_func(void* args){

    rwlock* lock = (rwlock*)args;

    for(int i = 0; i < 5000; i++){
        rwlock_acquire_read(lock);

        //FOR TESTING:
        //atomic_fetch_add(&readers_in_cs, 1);

        //stay for a bit longer
        for(volatile int j = 0; j < 1000; j++);

        //FOR TESTING:
        //atomic_fetch_sub(&readers_in_cs, 1);

        rwlock_release_read(lock);
        sched_yield();
    }
    return NULL;
}

int main(){
    printf("------ Starting Writer Preference Stress Test ------\n");

    //initialize 
    pthread_t readers[N1];
    pthread_t writers[N2];
    thread_args w_args[N2];
    rwlock lock;
    rwlock_init(&lock);

    //initialize writer success atomic array
    for (int i = 0; i < N2; i++) {
        atomic_init(&writer_success[i], 0);
    }

    //create 4 writers
    for (int i = 0; i < N2; i++){
        w_args[i].lock = &lock;
        w_args[i].id = i;
        if(pthread_create(&writers[i], NULL, writer_thread_func, &w_args[i]) != 0){
            perror("Failed to create writer thread");
            return 1;
        }
    }

    //create 12 readers
    for(int i = 0; i < N1; i++)
    {
        if(pthread_create(&readers[i], NULL, reader_thread_func, &lock) != 0){
            perror("Failed to create reader thread");
            return 1;
        }
    }

     //join the threads
    for(int i = 0; i < N1; i++)
    {
        pthread_join(readers[i], NULL);
    }

    for(int i = 0; i < N2; i++){
        pthread_join(writers[i], NULL);
    }

    int total_acquisitions = 0;
    int failed = 0;
    for(int i = 0; i < N2; i++){
        int acquisitions = atomic_load(&writer_success[i]);
        printf("Writer %d acquired the lock %d number of times.\n", i, acquisitions);
        total_acquisitions+= acquisitions;
        
        if (acquisitions < 100){
            failed = 1;
        }

    }
    printf("Total writer acquisitions: %d.\n", total_acquisitions);
    
    if (total_acquisitions < 5000){
        printf("TEST FAILED: Total acquisitions less than 5000.\n");
        return 1;
    }
    if (failed){
        printf("TEST FAILED: One or more writers were starved.\n");
        return 1;
    }

    //FOR TESTING PURPOSES
    //printf("\n=== Contention Report ===\n");
    //printf("Writers were forced to wait by active readers %d times.\n", atomic_load(&writer_blocked_by_reader));

    printf("TEST SUCCESSFUL: Writer preference stress test successful. \n");
    return 0;
}