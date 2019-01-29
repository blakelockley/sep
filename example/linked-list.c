//  === linked list exmaple ===
// Blake Lockley

#include "inst.h"
#include "run.h"
#include <stdio.h>

int main(int argc, char **argv) {

    //create a linked list of 3 nodes containing multiples of 3

    //allocate space for three seperate node instances
    ALLOC(2); //%0
    ALLOC(2); //%1
    ALLOC(2); //%2

    //set up first node
    //place value 7 at begining of node
    MUTATE_L(3, 0);
    //load address of "next pointer" in node
    GEP(0, 1); //%3
    //place address of second node in loaded address
    MUTATE(1, 3);

    //set up second node
    MUTATE_L(6, 1);
    GEP(1, 1); //%4
    MUTATE(2, 4);

    //set up last node
    MUTATE_L(9, 2);

    init();
    run();
    print_insts();
    print_regs();
    print_heap();

    delete_insts();
    delete_memory();
}
