#include "cond_var.h"
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    condition_variable* cv;
    ticket_lock* ext;
} ThreadArgs;

void handler(int sig) {
    printf("Caught signal %d (crash)\n", sig);
    exit(1);
}