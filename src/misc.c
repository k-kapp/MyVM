#include "../include/misc.h"
#include "../include/types.h"
#include <stdio.h>

void print_err(statement * stmt)
{
    if (stmt->type != ERROR)
        return;
    printf("Error on line %d: %s", stmt->int_arg, stmt->str_arg);
}
