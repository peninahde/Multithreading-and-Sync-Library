#include "cond_var.h"

void condition_variable_init(condition_variable* cv) {
    atomic_store(&cv->current_end, 0);
    atomic_store(&cv->current_thread, -1);
    ticketlock_init(&cv->lock);
}

void condition_variable_wait(condition_variable* cv, ticket_lock* ext_lock) {
    ticketlock_acquire(&cv->lock);
    int self_id = atomic_fetch_add(&cv->current_end, 1);
    ticketlock_release(&cv->lock);
    ticketlock_release(ext_lock);
    while(self_id > atomic_load(&cv->current_thread)) {
        sched_yield();
    }
    ticketlock_acquire(ext_lock);
}

void condition_variable_signal(condition_variable* cv) {
    ticketlock_acquire(&cv->lock);
    int current_end = atomic_load(&cv->current_end);
    int current_thread = atomic_load(&cv->current_thread);
    if (current_end == current_thread + 1) {
        ticketlock_release(&cv->lock);
        return;
    }
    atomic_fetch_add(&cv->current_thread, 1);
    ticketlock_release(&cv->lock);
}


void condition_variable_broadcast(condition_variable* cv) {
    ticketlock_acquire(&cv->lock);
    int new_start = atomic_load(&cv->current_end);
    new_start--;
    atomic_store(&cv->current_thread, new_start);
    ticketlock_release(&cv->lock);
}
