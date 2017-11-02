//Blake Lockley

#include <stdlib.h>

#ifndef INST_H
#define INST_H

#define ALLOC(n) ( add_node(new_alloc(n)) )
#define FREE(reg) ( add_node(new_free(reg)) )
#define LOOKUP(reg) ( add_node(new_lookup(reg)) )
#define MUTATE(regl, regr) ( add_node(new_mutate(regl, regr)) )
#define MUTATE_L(val,regr) ( add_node(new_mutate_l(val, regr)) )
#define LITERAL(value) ( add_node(new_literal(value)) )
#define ADD(lhs, rhs) ( add_node(new_add(lhs, rhs)) )

typedef enum {
    INST_ALLOC, INST_FREE, INST_MUTATE, INST_MUTATE_L, INST_LOOKUP, INST_LITERAL, INST_ADD
} inst_name_t;

typedef struct {
    inst_name_t name;
} inst_t;

typedef struct node {
    int num;
    inst_t *inst;
    struct node *next;
} node_t;

typedef struct {
    inst_name_t name;
    int size; //size of space to alloc on heap
} alloc_inst_t;

typedef struct {
    inst_name_t name;
    int reg; //reg containing address of space to free on heap
} free_inst_t;

typedef struct {
    inst_name_t name;
    int regl; //reg containing value to store
    int regr; //reg containing address to store value
} mutate_inst_t;

typedef struct {
    inst_name_t name;
    int val; //value to store
    int regr; //reg containing address to store value
} mutate_l_inst_t;

typedef struct {
    inst_name_t name;
    int reg; //reg containing address of space to load from heap
} lookup_inst_t;

typedef struct {
    inst_name_t name;
    int value; //reg containing address of space to load from heap
} literal_inst_t;

typedef struct {
    inst_name_t name;
    int lhs; //reg containing address of lhs
    int rhs; //reg containing address of rhs
} add_inst_t;

extern node_t *nodes;

void add_node(void *inst);
void print_insts(void);
void delete_insts(void);

alloc_inst_t *new_alloc(int size);
free_inst_t *new_free(int reg);
mutate_inst_t *new_mutate(int lval, int rval);
mutate_l_inst_t *new_mutate_l(int val, int rval);
lookup_inst_t *new_lookup(int reg);
literal_inst_t *new_literal(int value);
add_inst_t *new_add(int lhs, int rhs);

#endif /* INST_H */

