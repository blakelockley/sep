//Blake Lockley

#include "inst.h"
#include "run.h"
#include <stdio.h>

int main(int argc, char **argv) {

    //allocate space for one int and store 7 in that location
    ALLOC(1); //%0
    MUTATE_L(7, 0);

    //allocate space for one int and store 3 in that location
    ALLOC(1); //%1
    MUTATE_L(3, 1);

    //look up the two previous values, add them and store in the second location
    LOOKUP(0); //%2
    LOOKUP(1); //%3
    ADD(2, 3); //%4
    MUTATE(4, 1);

    //deallocate (only) the first memory location
    FREE(0);

    init();
    run();
    print_insts();
    print_regs();
    print_heap();

    delete_insts();
    //TODO: delete memory
}
