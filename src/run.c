//Blake Lockley

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "run.h"
#include "inst.h"

//a pseudo offset makes addresses some what more recgonisable
#define PSEUDO_OFFSET 0x1000

#pragma mark - heap

typedef struct heap_node {
    int address;
    int value;
    struct heap_node *prev;
    struct heap_node *next;
} heap_node_t;

heap_node_t *heap = NULL;

heap_node_t *new_heap_node(int address) {
    heap_node_t *node = malloc(sizeof(heap_node_t));
    node->value = 0;
    node->address = address;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

void delete_heap_node(heap_node_t *node) {
    if (node == NULL)
        return;
    delete_heap_node(node->next);
}

void print_heap(void) {
    heap_node_t *node = heap;

    printf("== HEAP ==\n");
    while (node != NULL) {
        printf("0x%x: 0x%x\n", node->address, node->value);
        node = node->next;
    }
}

#pragma mark - regs

int *regs = NULL;
int reg_size;
int reg_capacity;

void init_regs(int inital_capacity) {
    reg_size = 0;
    reg_capacity = inital_capacity;
    regs = malloc(sizeof(int) * inital_capacity);
}

void add_reg(int value) {
    if (reg_size == reg_capacity) {
        reg_capacity *= 2;
        regs = realloc(regs, reg_capacity);
    }
    
    regs[reg_size++] = value;
}

void print_regs(void) {
    printf("== REGISTERS ==\n");
    if (regs == NULL)
        return;
    for (int i = 0; i < reg_size; i++)
        printf("%%%d: 0x%x\n", i, regs[i]);
}

#pragma mark - exec

void init(void) {
    init_regs(10);
    srand((unsigned)time(NULL));
}

void exec_alloc(alloc_inst_t *inst) {
    if (inst->size <= 0)
        return;

    heap_node_t **dptr = &heap;
    heap_node_t *last = NULL;
    while (*dptr != NULL) {
        last = *dptr;
        dptr = &(*dptr)->next;
    }

    int address;
    if (last != NULL)
        address = last->address;
    else
        address = PSEUDO_OFFSET;
    address += (rand() % 0x40);

    for (int i = 0; i < inst->size; i++) {
        *dptr = new_heap_node(address + i);
        (*dptr)->prev = last;
        last = *dptr;
        dptr = &(*dptr)->next;
    }

    add_reg(address);
}

void exec_free(free_inst_t *inst) {
    heap_node_t *node = heap;
    while (node != NULL) {
        if (node->address == regs[inst->reg_addr]) {
            if (node == heap)
                heap = node->next;
            else
                node->prev->next = node->next;
            free(node);
            break;
        }
        node = node->next;
    }
}

void exec_mutate(mutate_inst_t *inst) {
    heap_node_t *node = heap;
    while (node != NULL) {
        if (node->address == regs[inst->reg_addr]) {
            node->value = regs[inst->reg_val];
            break;
        }
        node = node->next;
    }
}

void exec_mutate_l(mutate_l_inst_t *inst) {
    heap_node_t *node = heap;
    while (node != NULL) {
        if (node->address == regs[inst->reg_addr]) {
            node->value = inst->val;
            break;
        }
        node = node->next;
    }
}

void exec_lookup(lookup_inst_t *inst) {
    heap_node_t *node = heap;
    while (node != NULL) {
        if (node->address == regs[inst->reg_addr]) {
            add_reg(node->value);
            break;
        }
        node = node->next;
    }
}

void exec_literal(literal_inst_t *inst) {
    add_reg(inst->val);
}

void exec_add(add_inst_t *inst) {
    add_reg(regs[inst->reg_1] + regs[inst->reg_2]);
}

void exec_gep(gep_inst_t *inst) {
    add_reg(regs[inst->reg_addr] + inst->offset);
}

void run(void) {
    static node_t **node = &nodes;

    if (*node == NULL)
        return;
    
    switch ((*node)->inst->name) {
        case INST_ALLOC:
            exec_alloc((alloc_inst_t *) (*node)->inst);
            break;

        case INST_FREE:
            exec_free((free_inst_t *) (*node)->inst);
            break;

        case INST_MUTATE:
            exec_mutate((mutate_inst_t *) (*node)->inst);
            break;

        case INST_MUTATE_L:
            exec_mutate_l((mutate_l_inst_t *) (*node)->inst);
            break;

        case INST_LOOKUP:
            exec_lookup((lookup_inst_t *) (*node)->inst);
            break;

        case INST_LITERAL:
            exec_literal((literal_inst_t *) (*node)->inst);
            break;

        case INST_ADD:
            exec_add((add_inst_t *) (*node)->inst);
            break;

        case INST_GEP:
            exec_gep((gep_inst_t *) (*node)->inst);
            break;

        default:
            printf("ERROR: unkown node name\n");
            exit(1);
    }

    node = &(*node)->next;
    run();
}

void delete_memory(void) {
    free(regs);
    delete_heap_node(heap);
}
