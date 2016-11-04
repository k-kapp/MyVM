
#include "../include/scanner.h"
#include "../include/stack.h"
#include "../include/types.h"
#include "../include/parser.h"
#include "../include/symbol_table.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
int stmt_idx;
int errors_found;

short sign(int num)
{
    return num < 0 ? -1 : 1;
}

void execute_push(statement * stmt)
{
    if (main_stack_len >= main_stack_allocd)
    {
        main_stack = (int *)realloc(main_stack, sizeof(int)*(main_stack_allocd + main_stack_incr));
        main_stack_allocd += main_stack_incr;

    }
    if (stmt->str_arg)
    {
        main_stack[main_stack_len] = find_symbol(stmt->str_arg)->value;
    }
    else
    {
        main_stack[main_stack_len] = stmt->int_arg;
    }
    main_stack_len++;
}

void execute_pop(statement * stmt)
{
    if (stmt && stmt->str_arg)
    {
        int value = main_stack[main_stack_len - 1];
        add_symbol(stmt->str_arg, value);
    }
    main_stack_len--;
}

void execute_add()
{
    if (main_stack_len >= 2)
    {
        int answer = main_stack[main_stack_len - 1]
        + main_stack[main_stack_len - 2];
        main_stack[main_stack_len - 2] = answer;
        main_stack_len--;
    }
}

void execute_ifeq(statement * stmt)
{
    if (main_stack_len == 0)
        return;

    if (main_stack[main_stack_len - 1] == 0)
    {
        execute_jmp(stmt);
    }
    else
    {
        stmt_idx++;
        return;
    }
    //handle error code here
    stmt_idx++;
    errors_found = 1;
    return;
}

void execute_ifgt(statement * stmt)
{
    if (main_stack_len == 0)
        return;

    if (main_stack[main_stack_len - 1] > 0)
    {
        execute_jmp(stmt);
    }
    else
    {
        stmt_idx++;
        return;
    }
    //handle error code here
    stmt_idx++;
    errors_found = 1;
    return;
}

void execute_iflt(statement * stmt)
{
    if (main_stack_len == 0)
        return;

    if (main_stack[main_stack_len - 1] < 0)
    {
        execute_jmp(stmt);
    }
    else
    {
        stmt_idx++;
        return;
    }
    //handle error code here
    stmt_idx++;
    errors_found = 1;
    return;
}

void execute_jmp(statement * stmt)
{
    int i;
    for (i = 0; i < stmt_arr_len; i++)
    {
        if (stmts[i].type == LABEL && strcmp((char *)stmt->str_arg, (char *)stmts[i].str_arg) == 0)
        {
            stmt_idx = i;
            return;
        }
    }
    stmt_idx++;
    return;
}

void execute_print()
{
    if (main_stack_len > 0)
    {
        printf("%d\n", main_stack[main_stack_len - 1]);
    }
    else
    {
        printf("Stack empty. Nothing to print.\n");
    }

}

void execute_dup()
{
    statement push_stmt;
    push_stmt.int_arg = main_stack[main_stack_len - 1];
    push_stmt.str_arg = NULL;
    if (main_stack_len > 0)
    {
        execute_push(&push_stmt);
    }
}

void execute_mul()
{
    if (main_stack_len < 2)
        return;
    int times = main_stack[main_stack_len - 1];
    short times_sign = sign(times);
    times *= times_sign;
    execute_pop(NULL);
    int i;
    for (i = 0; i < times - 1; i++)
    {
        execute_dup();
    }
    for (i = 0; i < times - 1; i++)
    {
        execute_add();
    }
    main_stack[main_stack_len - 1] *= times_sign;
}

void execute_stmt(statement * stmt)
{
    switch (stmt->type)
    {
        case (PUSH):
        {
            execute_push(stmt);
            stmt_idx++;
        }
        break;
        case (POP):
        {
            execute_pop(stmt);
            stmt_idx++;
        }
        break;
        case (ADD):
        {
            execute_add();
            stmt_idx++;
        }
        break;
        case (IFEQ):
        {
            execute_ifeq(stmt);
        }
        break;
        case (JMP):
        {
            execute_jmp(stmt);
        }
        break;
        case (PRINT):
        {
            execute_print();
            stmt_idx++;
        }
        break;
        case (DUP):
        {
            execute_dup();
            stmt_idx++;
        }
        break;
        case (MUL):
        {
            execute_mul();
            stmt_idx++;
        }
        break;
        case (IFGT):
        {
            execute_ifgt(stmt);
        }
        break;
        case (IFLT):
        {
            execute_iflt(stmt);
        }
        break;
        default:
        {
            stmt_idx++;
        }
    }
}

void execute_program()
{
    if (errors_found)
    {
        return;
    }
    init_main_stack();
    stmt_idx = 0;
    while (stmt_idx < stmt_arr_len)
    {
        execute_stmt(&stmts[stmt_idx]);
    }
}

/*
void parse_stmt()
{
    int curr_idx;

    for (curr_idx = 0; curr_idx < stmt_arr_len; curr_idx++)
    {
        if (stmts[curr_idx])
    }
}
*/
