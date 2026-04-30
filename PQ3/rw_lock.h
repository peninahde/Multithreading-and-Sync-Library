#ifndef RW_LOCK_H
#define RW_LOCK_H

#include <stdatomic.h>
#include <sched.h>
#include "tl_semaphore.h"
// #include "cond_var.h" ADD THIS WHEN AVI MERGES

typedef struct 
{
    int active_readers;
    int active_writer; // 0 or 1
    int waiting_writers;

    ticket_lock inner_lock;

    //will have to edit this part according to what Avi named it
    cond_var cv; 

} rwlock;

#endif