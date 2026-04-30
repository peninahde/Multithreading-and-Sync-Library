/*
RW-lock must satisfy the following:
- Mutual exclusion for writers: if a writer holds the lock, no other writer or reader may hold it.
- Shared access for readers: if one or more readers hold the lock, no writer may hold it.
- Writer starvation prevention (required): if a writer starts waiting, it must eventually acquire the lock (assuming
threads continue to run and every thread that acquires the lock eventually releases it).
- Use writer-preference
*/
#include <stdatomic.h>
#include <sched.h>
#include <stdbool.h>
#include "tl_semaphore.h"
#include "cond_var.h"
#include "rw_lock.h"

void rwlock_init(rwlock* lock)
{
    lock->active_readers = 0;
    lock->active_writer = 0;
    lock->waiting_writers= 0;

    ticketlock_init(&lock->inner_lock); //ticket lock to protect our variables
}

void rwlock_acquire_read(rwlock* lock){
    //must acquire the lock in order to check the active_writer and waiting_writers
    ticketlock_acquire(&lock->inner_lock);

    //if there are any active or waiting writers, release the lock and go to sleep until woken (then re-acquires the lock)
    while(lock->active_writer == 1 || lock->waiting_writers > 0){
        condition_variable_wait(&lock->cv, &lock->inner_lock);
    }

    //now that we have the lock, we need to update the active_readers
    lock->active_readers++;

    //release the lock!
    ticketlock_release(&lock->inner_lock);

}

void rwlock_release_read(rwlock* lock){
    ticketlock_acquire(&lock->inner_lock);
    lock->active_readers--;

    if(lock->active_readers == 0){
        condition_variable_broadcast(&lock->cv);
    }

    ticketlock_release(&lock->inner_lock);
}

void rwlock_acquire_write(rwlock* lock){
    ticketlock_acquire(&lock->inner_lock);
    lock->waiting_writers++;
    while(lock->active_writer == 1 || lock->active_readers > 0){
        condition_variable_wait(&lock->cv, &lock->inner_lock);
    }
    lock->waiting_writers--;
    lock->active_writer = 1;
    ticketlock_release(&lock->inner_lock);
}



void rwlock_release_write(rwlock* lock){
/*
* Releases the lock after writing.
*/
    ticketlock_acquire(&lock->inner_lock);
    lock->active_writer = 0;

    //wake up the waiting writers
    condition_variable_broadcast(&lock->cv);

    ticketlock_release(&lock->inner_lock);

}
