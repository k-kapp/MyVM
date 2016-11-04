#include "../include/stack.h"
#include "../include/global_consts.h"
#include <stdlib.h>

int * main_stack;
int main_stack_incr = 10;
int main_stack_pos;
int main_stack_len;
int main_stack_allocd;

void init_main_stack()
{
    main_stack_pos = 0;
    main_stack_len = 0;
    main_stack_allocd = MAIN_STACK_INCR;
    main_stack = (int *)malloc(sizeof(int)*MAIN_STACK_INCR);
}
