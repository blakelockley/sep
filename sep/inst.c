//Blake Lockley

#include "inst.h"
#include <stdio.h>

node_t *nodes = NULL;

void add_node(void *inst) {
    static int count = 0;
    static node_t **next = &nodes;

    node_t *current = (node_t *) malloc(sizeof(node_t));
    current->num = count++;
    current->inst = inst;
    current->next = NULL;

    *next = current;
    next = &current->next;
}

void print_insts(void) {
    void print_node(node_t *node);

    printf("== INSTRUCTIONS ==\n");
    print_node(nodes);
}

void print_node(node_t *node) {
    if (node == NULL)
        return;

    printf("#%d: ", node->num);
    switch (node->inst->name) {
        case INST_ALLOC:
            printf("ALLOC $%d\n", ((alloc_inst_t*) node->inst)->size);
            break;

        case INST_FREE:
            printf("FREE %%%d\n", ((free_inst_t*) node->inst)->reg_addr);
            break;

        case INST_LOOKUP:
            printf("LOOKUP %%%d\n", ((lookup_inst_t*) node->inst)->reg_addr);
            break;

        case INST_MUTATE:
            printf("MUTATE %%%d ", ((mutate_inst_t*) node->inst)->reg_val);
            printf("%%%d\n", ((mutate_inst_t*) node->inst)->reg_addr);
            break;

        case INST_MUTATE_L:
            printf("MUTATE_L $%d ", ((mutate_l_inst_t*) node->inst)->val);
            printf("%%%d\n", ((mutate_l_inst_t*) node->inst)->reg_addr);
            break;

        case INST_LITERAL:
            printf("LITERAL $%d\n", ((literal_inst_t*) node->inst)->val);
            break;

        case INST_ADD:
            printf("ADD %%%d ", ((add_inst_t*) node->inst)->reg_1);
            printf("%%%d\n", ((add_inst_t*) node->inst)->reg_2);
            break;

        default:
            printf("ERROR: unkown node name\n");
            exit(1);
    }

    print_node(node->next);
}

alloc_inst_t *new_alloc(int size) {
    alloc_inst_t *result = (alloc_inst_t *) malloc(sizeof(alloc_inst_t));
    result->name = INST_ALLOC;
    result->size = size;
    return result;
}

free_inst_t *new_free(int reg_addr) {
    free_inst_t *result = (free_inst_t *) malloc(sizeof(free_inst_t));
    result->name = INST_FREE;
    result->reg_addr = reg_addr;
    return result;
}

mutate_inst_t *new_mutate(int reg_val, int reg_addr) {
    mutate_inst_t *result = (mutate_inst_t *) malloc(sizeof(mutate_inst_t));
    result->name = INST_MUTATE;
    result->reg_val = reg_val;
    result->reg_addr = reg_addr;
    return result;
}

mutate_l_inst_t *new_mutate_l(int val, int reg_addr) {
    mutate_l_inst_t *result = (mutate_l_inst_t *) malloc(sizeof(mutate_l_inst_t));
    result->name = INST_MUTATE_L;
    result->val = val;
    result->reg_addr = reg_addr;
    return result;
}

lookup_inst_t *new_lookup(int reg_addr) {
    lookup_inst_t *result = (lookup_inst_t *) malloc(sizeof(lookup_inst_t));
    result->name = INST_LOOKUP;
    result->reg_addr = reg_addr;
    return result;
}

literal_inst_t *new_literal(int val) {
    literal_inst_t *result = (literal_inst_t *) malloc(sizeof(literal_inst_t));
    result->name = INST_LITERAL;
    result->val = val;
    return result;
}

add_inst_t *new_add(int reg_1, int reg_2) {
    add_inst_t *result = (add_inst_t *) malloc(sizeof(add_inst_t));
    result->name = INST_ADD;
    result->reg_1 = reg_1;
    result->reg_2 = reg_2;
    return result;
}

void delete_node(node_t *node) {
    if (node == NULL)
        return;

    delete_node(node->next);
    free(node->inst);
    free(node);
}

void delete_insts(void) {
    delete_node(nodes);
}
