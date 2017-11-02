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
            printf("FREE %%%d\n", ((free_inst_t*) node->inst)->reg);
            break;

        case INST_LOOKUP:
            printf("LOOKUP %%%d\n", ((lookup_inst_t*) node->inst)->reg);
            break;

        case INST_MUTATE:
            printf("MUTATE %%%d ", ((mutate_inst_t*) node->inst)->regl);
            printf("%%%d\n", ((mutate_inst_t*) node->inst)->regr);
            break;

        case INST_MUTATE_L:
            printf("MUTATE_L $%d ", ((mutate_inst_t*) node->inst)->regl);
            printf("%%%d\n", ((mutate_inst_t*) node->inst)->regr);
            break;

        case INST_LITERAL:
            printf("LITERAL $%d\n", ((literal_inst_t*) node->inst)->value);
            break;

        case INST_ADD:
            printf("ADD %%%d ", ((add_inst_t*) node->inst)->lhs);
            printf("%%%d\n", ((add_inst_t*) node->inst)->rhs);
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

free_inst_t *new_free(int reg) {
    free_inst_t *result = (free_inst_t *) malloc(sizeof(free_inst_t));
    result->name = INST_FREE;
    result->reg = reg;
    return result;
}

mutate_inst_t *new_mutate(int lval, int rval) {
    mutate_inst_t *result = (mutate_inst_t *) malloc(sizeof(mutate_inst_t));
    result->name = INST_MUTATE;
    result->regl = lval;
    result->regr = rval;
    return result;
}

mutate_l_inst_t *new_mutate_l(int val, int rval) {
    mutate_l_inst_t *result = (mutate_l_inst_t *) malloc(sizeof(mutate_l_inst_t));
    result->name = INST_MUTATE_L;
    result->val = val;
    result->regr = rval;
    return result;
}

lookup_inst_t *new_lookup(int reg) {
    lookup_inst_t *result = (lookup_inst_t *) malloc(sizeof(lookup_inst_t));
    result->name = INST_LOOKUP;
    result->reg = reg;
    return result;
}

literal_inst_t *new_literal(int value) {
    literal_inst_t *result = (literal_inst_t *) malloc(sizeof(literal_inst_t));
    result->name = INST_LITERAL;
    result->value = value;
    return result;
}

add_inst_t *new_add(int lhs, int rhs) {
    add_inst_t *result = (add_inst_t *) malloc(sizeof(add_inst_t));
    result->name = INST_ADD;
    result->rhs = lhs;
    result->lhs = rhs;
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
