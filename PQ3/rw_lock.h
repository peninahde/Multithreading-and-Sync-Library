#ifndef RW_LOCK_H
#define RW_LOCK_H

#include <stdatomic.h>
#include <sched.h>
#include "tl_semaphore.h"
#include "cond_var.h" 

typedef struct 
{
    int active_readers;
    int active_writer; // 0 or 1
    int waiting_writers;

    ticket_lock inner_lock;

    //will have to edit this part according to what Avi named it
    condition_variable cv; 

} rwlock; 

/*
* Initializes the read-write lock.
*/
void rwlock_init(rwlock* lock);

/*
* Acquires the lock for reading.
* can only get the lock if there are no writers because of writer preference 
*/
void rwlock_acquire_read(rwlock* lock);

/*
* Releases the lock after reading.
*/
void rwlock_release_read(rwlock* lock);

/*
* Acquires the lock for writing (exclusive access).
*/
void rwlock_acquire_write(rwlock* lock);

#endif