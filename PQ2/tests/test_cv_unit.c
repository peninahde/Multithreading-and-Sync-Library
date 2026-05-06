#include "tests.h"

int main() {
    signal(SIGSEGV, handler);
    condition_variable* cnd = malloc(sizeof(condition_variable));
    condition_variable_init(cnd);
    condition_variable_signal(cnd);
    condition_variable_broadcast(cnd);
    return 0;
}