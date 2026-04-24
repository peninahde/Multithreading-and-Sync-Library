#include "cond_var.h"

void condition_variable_init(condition_variable* cv) {
    atomic_store(&cv->current_end, 0);
    atomic_store(&cv->current_thread, -1);
}

void condition_variable_wait(condition_variable* cv, ticket_lock* ext_lock) {
    int self_index = atomic_fetch_add(&cv->current_end, 1);
    ticketlock_release(ext_lock);
    while(self_index > atomic_load(&cv->current_thread)) {
        sched_yield();
    }
    ticketlock_acquire(ext_lock);
}

void condition_variable_signal(condition_variable* cv) {
    int current_end = atomic_load(&cv->current_end);
    int current_thread = atomic_load(&cv->current_thread);
    if (current_end == current_thread + 1) {
        return;
    }
    atomic_fetch_add(&cv->current_thread, 1);
}


void condition_variable_broadcast(condition_variable* cv) {
    int new_start = atomic_load(&cv->current_end);
    new_start--;
    atomic_store(&cv->current_thread, new_start);
}
