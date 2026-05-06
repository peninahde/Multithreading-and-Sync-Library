#!/bin/bash

cd "$(dirname "$0")"

# check that the output directory exists
mkdir -p output
EXIT_STATUS=0

# build the tests
for test_file in tests/test_*.c; do
    test_name=$(basename "$test_file" .c)
    echo "Compiling $test_name..."

    gcc -std=c17 -Wall -Wextra -Werror -pedantic -g -pthread -I. \
        "$test_file" tl_semaphore.c cond_var.c rw_lock.c\
        -o "output/$test_name"
    
    if [ $? -ne 0 ]; then
        echo "Compilation of $test_name failed!"
        EXIT_STATUS=1
    fi

done

# short circuit if something failed to compile
if [ $EXIT_STATUS -ne 0 ]; then
    echo "Aborting due to compilation errors."
    exit 1
fi

# now run the tests
for binary in output/test_*; do
    test_name=$(basename "$binary")
    echo "Running $test_name..."

    "./$binary"
    if [ $? -eq 0 ]; then
        echo "$test_name PASSED YAY!"
    else 
        echo "$test_name FAILED :("
        EXIT_STATUS=1
    fi
done

exit $EXIT_STATUS
