
#include "types.h"

enum class Op: UInt16 {
    ADD,
    AND,
    CMP,
    DIV,
    HLT,
    JMP,
    MUL,
    NOT,
    OR,
    RET,
    SUB,
    XOR,
};

typedef struct { char data[4]; } OpKey;
const OpKey opkeys[] = {
    {"ADD"},
    {"AND"},
    {"CMP"},
    {"DIV"},
    {"HLT"},
    {"JMP"},
    {"MUL"},
    {"NOT"},
    {"OR" },
    {"RET"},
    {"SUB"},
    {"XOR"},
};



