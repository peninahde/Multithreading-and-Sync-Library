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
#include "rw_lock.h"

void rwlock_init(rwlock* lock)
{
/*
* Initializes the read-write lock.
*/
    lock->active_readers = 0;
    lock->active_writer = 0;
    lock->waiting_writers= 0;

    ticketlock_init(&lock->inner_lock); //ticket lock to protect our variables
}

void rwlock_acquire_read(rwlock* lock);
/*
* Acquires the lock for reading.
*/


void rwlock_release_read(rwlock* lock);
/*
* Releases the lock after reading.
*/


void rwlock_acquire_write(rwlock* lock);
/*
* Acquires the lock for writing (exclusive access).
*/


void rwlock_release_write(rwlock* lock);
/*
* Releases the lock after writing.
*/