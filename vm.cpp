
#include <cstdio>
#include <cstdlib>
#include "types.h"

void abort(const char* cstr) {
    std::printf("\033[1;31mFAILURE\033[0m: %s\n");
    std::exit(1);
}
typedef Int64 Value;
////////////////////////////////////////////////////////////////////////////////
typedef enum {
    NodeKind_SET,
    NodeKind_VALUE,
    NodeKind_VAR,
} NodeKind;
typedef struct {
    NodeKind kind;
    union {
        UInt8 argc;
        UInt8 index;
    } data_as;
} Node;

////////////////////////////////////////////////////////////////////////////////
constexpr UInt16 STACK_CAP = 1024;
Value stack[STACK_CAP];
UInt16 stack_count = 0;

/*typedef struct {
    UInt16 start;
} Scope;
constexpr UInt16 CALLSTACK_CAP = 32;
Scope callstack[CALLSTACK_CAP];
UInt16 frame = 0;*/

typedef enum {
    OpRET,
    OpSETVALUE,
} OpKind;
typedef struct {
    OpKind kind;
    union {
        struct { UInt8 a; UInt8 b; UInt8 c; } args3;
        struct { UInt8 a; UInt16 b; } args2x;
    } data_as;
} Op;

#define ENCODE_OP3(_KIND, _A, _B, _C)\
    ((Op){ (_KIND), {.args3 = {(_A), (_B), (_C)}} })

#define ENCODE_OP2X(_KIND, _A, _B)\
    ((Op){ (_KIND), {.args2x = {(_A), (_B)}} })

#define ENCODE_SETVALUE(_REG, _VALUE) ENCODE_OP2X(OpSETVALUE, _REG, _VALUE)
#define ENCODE_RET(_REG) ENCODE_OP3(OpRET, _REG, 0, 0)

struct Program {
    Op* ops = nullptr;
    UInt32 cap = 0;
    UInt32 count = 0;

    void alloc(UInt32 new_cap) {
        Op* new_alloc = (Op*)std::realloc((void*)ops, new_cap * sizeof(Op));
        if (new_alloc == nullptr) abort("Bad allocation.\n");
        if (new_cap < count) count = new_cap;
        ops = new_alloc;
        cap = new_cap;
    }
    void free(void) {
        std::free(ops);
        ops = nullptr;
        cap = 0;
        count = 0;
    }

    void write(Op op) {
        if (cap < (count + 1)) alloc(cap * 2);
        ops[count] = op;
        count += 1;
    }
};

void compile_node(Program program, const Node* nodes, const UInt32 nodes_count, UInt32& index) {
    switch(nodes[index].kind) {
      default: abort("Unknown node.");
      case NodeKind_SET: {
        if (nodes[index].data_as.argc != 2) abort("Unexpected argc.");
        index++;

        if (nodes[index].kind != NodeKind_VAR) abort("Expected variable.");
        UInt8 var = nodes[index].data_as.index;
        index++;

        if (nodes[index].kind != NodeKind_VALUE) abort("Expected value.");
        UInt8 value = nodes[index].data_as.index;
        index++;

        program.write(ENCODE_SETVALUE(var, value));
      } break;
    }
}

Program compile(const Node* nodes, const UInt32 nodes_count) {
    Program program;
    program.alloc(1024);
    UInt32 index = 0;
    while (index < nodes_count) compile_node(program, nodes, nodes_count, index);
    return program;
}

Value constants[] = {
    420,
    505,
};
Node nodes[] = {
    {NodeKind_SET, {.argc = 2}},
        {NodeKind_VAR, {.index = 0}},
        {NodeKind_VALUE, {.index = 0}},
};

Int32 main(void) {
    Program program = compile(nodes, sizeof(nodes)/sizeof(Node));
    for (UInt32 i = 0; i < program.count; i++)
        std::printf("%d\n", (UInt32)program.ops[i]);

    return (UInt32)ENCODE_SETVALUE(0, 0);
}