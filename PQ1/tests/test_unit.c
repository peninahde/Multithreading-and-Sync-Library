//  Unit test (single thread): basic init/signal/wait behavior (allowed to use pthread!)
#include <stdatomic.h>
#include <sched.h>
#include <stdio.h>
#include <assert.h>
#include "../tl_semaphore.h"

int main()
{
    semaphore sem;
    //some helpful prints so we know what's going on you know?
    printf("----Starting Unit Test ----\n");

    //Test Initialization
    semaphore_init(&sem, 1);
    //check if it actually worked
    assert(sem.value == 1);
    printf("Init worked.\n");

    //Test Wait
    semaphore_wait(&sem);
    assert(sem.value == 0);
    printf("Wait works.\n");

    //Test Signal
    semaphore_signal(&sem);
    assert(sem.value == 1);
    printf("Signal works.\n");

    printf("----- All functions successful -----\n");
    return 0;
}