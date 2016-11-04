
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef struct symbol_
{
    char * name;
    int value;
} symbol;

extern symbol * symbol_table;
extern int symbol_table_len;
extern int symbol_table_allocd;

void init_symbol_table();
void add_symbol(char * name, int value);
symbol * find_symbol(char * name);
short modify_symbol(char * name, int value);
int retrieve_symbol_value(char * name);

#endif // SYMBOL_TABLE_H
