
#ifndef TYPES_H
#define TYPES_H


typedef enum ops_
{
    PUSH,
    POP,
    ADD,
    IFEQ,
    JMP,
    PRINT,
    DUP,
    MUL,
    IFGT,
    IFLT,
    LABEL,
    ERROR
} op;

typedef struct statement_
{
    op type;
    int int_arg;
    char * str_arg;
} statement;

#endif // TYPES_H
