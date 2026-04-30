#include <stdatomic.h>
#include <sched.h>
#include "tl_semaphore.h"

//implement the ticket lock like we saw in the lecture for the semaphore to use later
void ticketlock_init(ticket_lock* lock) 
{
    atomic_init(&lock->ticket, 0);
    atomic_init(&lock->cur_ticket, 0);
}

void ticketlock_acquire(ticket_lock* lock) 
{
    int my_ticket = atomic_fetch_add(&lock->ticket, 1);
    while (atomic_load(&lock->cur_ticket) != my_ticket) {
        sched_yield();
    }
}

void ticketlock_release(ticket_lock* lock) 
{
    atomic_fetch_add(&lock->cur_ticket, 1);
}

//mandatory semaphore functions
/* Initializes the semaphore with an initial value (initial_value >= 0). */
void semaphore_init(semaphore* sem, int initial_value)
{
    sem->value = initial_value;
    ticketlock_init(&sem->lock); //initialize the lock to use with the counter
}   

void semaphore_wait(semaphore* sem) // Decrement (wait / P)
{
    while (1)
    {
        // If value > 0 , decrement value by 1 and return.
        ticketlock_acquire(&sem->lock); //get the lock so only this can look at it rn
        if(sem->value > 0)
        {
            sem->value--;
            ticketlock_release(&sem->lock);
            return;
        }

        // If value == 0 , the calling thread must wait until value > 0 , then decrement and return
        // Waiting must be implemented by looping and calling sched_yield() (i.e., no blocking primitives).
        ticketlock_release(&sem->lock);
        sched_yield();
    }
} 
   
void semaphore_signal(semaphore* sem) // Increment (signal / V)
{
    // Increment value by 1.
    // This must eventually allow waiting thread(s) to complete semaphore_wait().
    ticketlock_acquire(&sem->lock);
    sem->value++;
    ticketlock_release(&sem->lock); //release the lock so other threads can complete the semaphore_wait() :)
} 
