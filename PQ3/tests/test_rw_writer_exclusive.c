/*
- Create 1 writer thread that acquires the write lock and, while holding it, checks that:
    - no readers are inside (a shared atomic readers_in_cs must be 0)
    - no other writer is inside
- In parallel, create 8 reader threads trying to read repeatedly.
- Verify the writer never overlaps with any reader/writer in the critical section.

return 0 on success
return 1 on failure
(Printing a short failure message is recommended.)
*/

#include <stdio.h>
#include <stdatomic.h>
#include <sched.h>
#include <pthread.h>
#include "../rw_lock.h"

#define N 8

atomic_int readers_in_cs = 0;
atomic_int writer_in_cs = 0;
atomic_int test_failed = 0;

void* writer_thread_func(void* args){

    rwlock* lock = (rwlock*)args;
    rwlock_acquire_write(lock);

    //mark we have one writer inside now
    atomic_fetch_add(&writer_in_cs, 1);

    //check that no one else has entered
    int current_readers = atomic_load(&readers_in_cs);
    int current_writers = atomic_load(&writer_in_cs);

    if (current_readers > 0 || current_writers > 1){
        printf("TEST FAILED: Writer overlapped with %d Readers, %d Writers \n", current_readers, current_writers);
        atomic_store(&test_failed, 1);
    }

    //wait long enough to catch any readers
    for(volatile int i = 0; i < 2000000; i++);

    atomic_fetch_sub(&writer_in_cs, 1);
    rwlock_release_write(lock);

    return NULL;
}

void* reader_thread_func(void* args){

    rwlock* lock = (rwlock*)args;

    for(int i = 0; i < 100; i++){
        rwlock_acquire_read(lock);

        atomic_fetch_add(&readers_in_cs, 1);

        //if the reader got in and there is still a writer there, test fails
        if(atomic_load(&writer_in_cs) > 0) {
            atomic_store(&test_failed,1);
        }

        //stay for a bit longer
        for(volatile int j = 0; j < 1000; j++);

        atomic_fetch_sub(&readers_in_cs, 1);
        rwlock_release_read(lock);
    }
    return NULL;
}


int main(){
    pthread_t readers[N];
    pthread_t writer;
    rwlock lock;

    printf("------ Starting Writer Exclusion Test ------\n");

    //initialize the lock
    rwlock_init(&lock); 
    atomic_init(&readers_in_cs, 0);
    atomic_init(&writer_in_cs, 0);
    atomic_init(&test_failed, 0);

    //create 1 writer
    pthread_create(&writer, NULL, writer_thread_func, &lock);

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

    pthread_join(writer, NULL);

    if (atomic_load(&test_failed) == 1){
        printf("----TEST FAILED: Overlap was detected :( ----\n");
        return 1;
    } 

    printf("TEST SUCCESSFUL: Writer exclusion successful. \n");
    return 0;
}