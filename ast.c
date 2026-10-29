
#include <stdlib.h>
#include <stdio.h>
#include "types.h"

void error(const char *cstr) {
    printf("\033[1;31mFAILURE\033[0m: %s\n");
    exit(1);
};

typedef enum: UInt8 {
    // NodeKind_
    NodeKind_RETURN,
    NodeKind_SET,
    NodeKind_VALUE,
    NodeKind_VAR,
} NodeKind;

typedef struct {
    NodeKind kind;
    UInt32 data: 24;
} Node;
typedef struct {
    const Node* nodes;
    const UInt64 count;
} Ast;


typedef enum {
    OpKind_SETVALUE,
    OpKind_RET,
} OpKind;
typedef struct {
    OpKind kind;
    union {
        struct { UInt8 a; UInt8 b; UInt8 c; } args3;
        struct { UInt8 a; UInt16 b; } args2x;
    } data_as;
} Op;

typedef struct {
    Op* ops;
    UInt32 count;
    UInt32 cap;
} Program;

void program_alloc(Program* program, UInt32 new_cap) {
    if (new_cap < program->count) error("Allocating less then stored.");
    Op* new_alloc = realloc(program->ops, new_cap);
    if (new_alloc == NULL) error("Bad allocation.\n");
    program->ops = new_alloc;
    program->cap = new_cap;
}
void program_push(Program* program, Op op) {
    if (program->count + 1 > program->cap)
        program_alloc(program, program->cap * 2);
    program->ops[program->count] = op;
    program->count += 1;
}

void compile_node(Ast ast, Program* program, UInt32* index) {
    switch (ast.nodes[*index]) {
      case 

    }
}


Program compile(Ast ast) {
    Program program = (Program) {
        .ops = NULL,
        .count = 0,
        .cap = 0,
    }; 
    program_alloc(&program, 100);
    UInt32 index = 0;
    while (index < ast.count)
        compile_node(ast, &program, &index);
    return program;
}




//int variables[];

typedef Int64 Value;
Value constants[] = {
    505,
    420,
};
Node nodes[] = {
    {NodeKind_SET, 2},
        {NodeKind_VAR, 0},
        {NodeKind_VALUE, 1},

    {NodeKind_SET, 2},
        {NodeKind_VAR, 1},
        {NodeKind_VALUE, 0},

    {NodeKind_RETURN, 1},
        {NodeKind_VAR, 0},
};

Int32 main(void) {
    Ast ast = (Ast){ nodes, sizeof(nodes)/sizeof(Node) };
    


    return sizeof(Node);
}