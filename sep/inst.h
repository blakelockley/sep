//Blake Lockley

#include <stdlib.h>

#ifndef INST_H
#define INST_H

#define ALLOC(size) ( add_node(new_alloc(size)) )
#define FREE(reg_addr) ( add_node(new_free(reg_addr)) )
#define LOOKUP(reg_addr) ( add_node(new_lookup(reg_addr)) )
#define MUTATE(reg_val, reg_addr) ( add_node(new_mutate(reg_val, reg_addr)) )
#define MUTATE_L(val, reg_addr) ( add_node(new_mutate_l(val, reg_addr)) )
#define LITERAL(val) ( add_node(new_literal(val)) )
#define ADD(reg_1, reg_2) ( add_node(new_add(reg_1, reg_2)) )
#define GEP(reg_addr, offset) ( add_node(new_gep(reg_addr, offset)) )

typedef enum {
    INST_ALLOC, INST_FREE, INST_MUTATE, INST_MUTATE_L, INST_LOOKUP, INST_LITERAL, INST_ADD, INST_GEP
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
    int reg_addr; //reg containing address of space to free on heap
} free_inst_t;

typedef struct {
    inst_name_t name;
    int reg_val; //reg containing value to store
    int reg_addr; //reg containing address to store value
} mutate_inst_t;

typedef struct {
    inst_name_t name;
    int val; //value to store
    int reg_addr; //reg containing address to store value
} mutate_l_inst_t;

typedef struct {
    inst_name_t name;
    int reg_addr; //reg containing address of space to load from heap
} lookup_inst_t;

typedef struct {
    inst_name_t name;
    int val; //reg containing address of space to load from heap
} literal_inst_t;

typedef struct {
    inst_name_t name;
    int reg_1; //reg containing address of lhs
    int reg_2; //reg containing address of rhs
} add_inst_t;

typedef struct {
    inst_name_t name;
    int reg_addr; //reg containing address of lhs
    int offset; //reg containing address of rhs
} gep_inst_t;

extern node_t *nodes;

void add_node(void *inst);
void print_insts(void);
void delete_insts(void);

alloc_inst_t *new_alloc(int size);
free_inst_t *new_free(int reg_addr);
mutate_inst_t *new_mutate(int reg_val, int reg_addr);
mutate_l_inst_t *new_mutate_l(int val, int reg_addr);
lookup_inst_t *new_lookup(int reg_addr);
literal_inst_t *new_literal(int val);
add_inst_t *new_add(int reg_1, int reg_2);
gep_inst_t *new_gep(int reg_addr, int offset);

#endif /* INST_H */

