
#include "../include/symbol_table.h"
#include "../include/global_consts.h"
#include <stdlib.h>
#include <string.h>

symbol * symbol_table;
int symbol_table_len;
int symbol_table_allocd;

void init_symbol_table()
{
    symbol_table_len = 0;
    symbol_table_allocd = SYMBOL_TABLE_INCR;
    symbol_table = (symbol *)malloc(sizeof(symbol)*SYMBOL_TABLE_INCR);
}

void add_symbol(char * name, int value)
{
    if (modify_symbol(name, value))
    {
        return;
    }
    if (symbol_table_len >= symbol_table_allocd)
    {
        symbol_table = (symbol *)realloc(symbol_table, sizeof(symbol)*(symbol_table_allocd + SYMBOL_TABLE_INCR));
        symbol_table_allocd += SYMBOL_TABLE_INCR;
    }
    symbol_table[symbol_table_len].name = (char *)malloc(sizeof(char)*MAX_IDENT_LENGTH);
    strcpy(symbol_table[symbol_table_len].name, name);
    symbol_table[symbol_table_len].value = value;
    symbol_table_len++;
}

symbol * find_symbol(char * name)
{
    int i;
    for (i = 0; i < symbol_table_len; i++)
    {
        if (strcmp(name, symbol_table[i].name) == 0)
        {
            return &symbol_table[i];
        }
    }
    return NULL;
}

short modify_symbol(char * name, int value)
{
    symbol * sym = find_symbol(name);
    if (sym)
    {
        sym->value = value;
        return 1;
    }
    return 0;
}

int retrieve_symbol_value(char * name)
{
    return find_symbol(name)->value;
}
