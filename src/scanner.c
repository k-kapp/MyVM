#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "../include/global_consts.h"
#include "../include/misc.h"
#include "../include/types.h"
#include "../include/parser.h"
#include "../include/symbol_table.h"

const char * reserved_words [] = {"push", "pop", "add", "ifeq", "jmp", "print", "dup", "mul", "ifgt", "iflt"};
statement * stmts;
int stmt_arr_len;
int stmt_arr_allocd;

void init_stmts()
{
    stmt_arr_len = 0;
    stmts = (statement *)malloc(sizeof(statement)*STMT_ARR_INCR);
    stmt_arr_allocd = STMT_ARR_INCR;
}

void add_stmt(statement * stmt)
{
    if (stmt_arr_len >= stmt_arr_allocd)
    {
        stmts = realloc(stmts, sizeof(statement)*(stmt_arr_allocd + STMT_ARR_INCR));
        stmt_arr_allocd += STMT_ARR_INCR;
    }
    stmts[stmt_arr_len] = *stmt;
    stmt_arr_len++;
}

short in_keywords(char * stmt)
{
    char * keyword = (char *)malloc(sizeof(char)*MAX_KEYWORD_LEN);
    int i = 0;
    while (isgraph(stmt[i]))
    {
        i++;
    }
    if (i > MAX_KEYWORD_LEN)
        return 0;
    strncpy(keyword, stmt, i);
    keyword[i] = '\0';
    for (i = 0; i < NUM_RESERVED; i++)
    {
        if (strcmp(keyword, reserved_words[i]) == 0)
            return 1;
    }
    return 0;
}

short in_reserved(char * word)
{
    int i;
    for (i = 0; i < NUM_RESERVED; i++)
    {
        if (strcmp(reserved_words[i], word) == 0)
        {
            return 1;
        }
    }
    return 0;
}

void trim_stmt(char ** stmt_ptr)
{
    char * stmt = *stmt_ptr;
    int read_idx = 0;
    int last_graph, first_graph;
    while (!isgraph(stmt[read_idx]))
    {
        read_idx++;
    }
    first_graph = read_idx;
    while (stmt[read_idx] != '\0')
    {
        read_idx++;
    }
    read_idx--;
    while (!isgraph(stmt[read_idx]))
    {
        read_idx--;
    }
    last_graph = read_idx;
    char * new_stmt = (char *)malloc(sizeof(char)*MAX_STMT_LENGTH);
    read_idx = first_graph;
    int write_idx;
    for (write_idx = 0; write_idx < last_graph - first_graph + 1; write_idx++, read_idx++)
    {
        new_stmt[write_idx] = stmt[read_idx];
    }
    new_stmt[write_idx] = '\0';
    free(stmt);
    *stmt_ptr = new_stmt;
}

char * extract_arg(char * stmt, int read_idx)
{
    char * str_arg = (char *)malloc(sizeof(char *)*33);
    int write_idx = 0;
    while ((stmt[read_idx] != '\0') && !isgraph(stmt[read_idx]))
    {
        read_idx++;
    }
    while ((stmt[read_idx] != '\0') && isgraph(stmt[read_idx]))
    {
        str_arg[write_idx] = stmt[read_idx];
        read_idx++;
        write_idx++;
    }
    str_arg[write_idx] = '\0';
    return str_arg;
}

short str_is_numeric(char * str)
{
    short digits_found = 0;
    int read_idx = 0;
    int first_graph = 0;
    while ((str[read_idx] != '\0') && !isgraph(str[read_idx]))
        read_idx++;
    first_graph = read_idx;
    while (str[read_idx] != '\0')
    {
        if ((str[read_idx] == '-') && (read_idx == first_graph))
        {
        }
        else if (!isdigit(str[read_idx]))
        {
            return 0;
        }
        else if (isdigit(str[read_idx]))
        {
            digits_found = 1;
        }
        read_idx++;
    }
    return digits_found;
}

op identify_statement(char * stmt_text)
{
    int i;
    for (i = 0; i < NUM_RESERVED; i++)
    {
        if (strcmp(reserved_words[i], stmt_text) == 0)
        {
            return (op)i;
        }
    }
    return 0;
}

statement * validate_stmt(char * stmt, int line_number)
{
    //printf("Validating line number %d\n", line_number);
    int read_idx = 0;
    int write_idx = 0;
    char * text = (char * )malloc(sizeof(char)*256);
    while (stmt[read_idx] != '\0' && !isgraph(stmt[read_idx]))
    {
        read_idx++;
    }
    if (stmt[read_idx] == '\0')
    {
        //printf("Returning null\n");
        return NULL;
    }
    while (isgraph(stmt[read_idx]))
    {
        text[write_idx] = stmt[read_idx];
        write_idx++;
        read_idx++;
    }
    text[write_idx] = '\0';
    if (!in_keywords(text))
    {
        if (text[write_idx - 1] != ':')
        {
            statement * label_err = (statement *)malloc(sizeof(statement));
            label_err->str_arg = (char *)malloc(sizeof(char)*MAX_IDENT_LENGTH);
            sprintf(label_err->str_arg, "Label %s must end with ':'\n", text);
            label_err->int_arg = line_number;
            label_err->type = ERROR;
            free(text);
            return label_err;
        }
        else
        {
            text[write_idx - 1] = '\0';
            if (in_keywords(text))
            {
                statement * label_err = (statement *)malloc(sizeof(statement));
                label_err->str_arg = "Label cannot be reserved keyword\n";
                label_err->int_arg = line_number;
                label_err->type = ERROR;
                free(text);
                return label_err;
            }
            else
            {
                statement * label = (statement *)malloc(sizeof(statement));
                label->str_arg = text;
                label->type = LABEL;
                return label;
            }
        }
    }
    else
    {
        statement * keyw_stmt = (statement *)malloc(sizeof(statement));
        keyw_stmt->type = identify_statement(text);
        switch (keyw_stmt->type)
        {
            case (PUSH):
            {
                char * arg = extract_arg(stmt, read_idx);
                if (arg[0] == '\0')
                {
                    keyw_stmt->type = ERROR;
                    keyw_stmt->str_arg = "Required: PUSH statement requires integer as argument\n";
                    keyw_stmt->int_arg = line_number;
                }
                else if (!str_is_numeric(arg))
                {
                    symbol * ident = find_symbol(arg);
                    if (!ident)
                    {
                        keyw_stmt->type = ERROR;
                        keyw_stmt->str_arg = (char *)malloc(sizeof(char)*32);
                        sprintf(keyw_stmt->str_arg, "Unknown identifier '%s'\n", arg);
                        keyw_stmt->int_arg = line_number;
                    }
                    else
                    {
                        keyw_stmt->type = PUSH;
                        keyw_stmt->str_arg = arg;
                    }
                }
                else
                {
                    keyw_stmt->type = PUSH;
                    keyw_stmt->int_arg = atoi(arg);
                    keyw_stmt->str_arg = NULL;
                }
            }
            break;
            case (POP):
            {
                char * arg = extract_arg(stmt, read_idx);
                if (arg[0] != '\0')
                {
                    if (!str_is_numeric(arg))
                    {
                        keyw_stmt->type = POP;
                        keyw_stmt->str_arg = arg;
                        add_symbol(keyw_stmt->str_arg, 0);
                    }
                    else
                    {
                        printf("argument: |%s|\n", arg);
                        printf("Statement: |%s|\n", stmt);
                        keyw_stmt->type = ERROR;
                        keyw_stmt->str_arg = "POP must have an identifier as argument, or none\n";
                        keyw_stmt->int_arg = line_number;
                    }
                }
                else
                {
                    keyw_stmt->type = POP;
                    keyw_stmt->str_arg = NULL;
                }
            }
            break;
            case (ADD):
            {
                keyw_stmt->type = ADD;
            }
            break;
            case (IFGT):
            case (IFLT):
            case (IFEQ):
            {
                char * arg = extract_arg(stmt, read_idx);
                keyw_stmt->str_arg = arg;
            }
            break;
            case (JMP):
            {
                char * arg = extract_arg(stmt, read_idx);
                keyw_stmt->type = JMP;
                keyw_stmt->str_arg = arg;
            }
            break;
            case (PRINT):
            {
                keyw_stmt->type = PRINT;
            }
            break;
            case (DUP):
            {
                keyw_stmt->type = DUP;
            }
            break;
            case (MUL):
            {
                keyw_stmt->type = MUL;
            }
            break;
            default:
            {
            }
        }
        return keyw_stmt;
    }
}


/*
void rm_dupl_spaces(char ** stmt_ptr)
{
    char * stmt = *stmt_ptr;
    int read_idx = 0;
    int end_first = -1;
    int start_second = -1;
    while (stmt[read_idx] != '\0')
    {
        if (!isgraph(stmt[read_idx]))
        {
            if (end_first == -1)
            {
                end_first =
            }
        }
        read_idx++;
    }
    if (end_first == -1 && start_second == -1)
    {
        return;
    }
    char * new_stmt = (char *)malloc(sizeof(char)*MAX_STMT_LENGTH);
    int write_idx;
    for (write_idx = 0; write_idx < end_first; write_idx++)
    {
        new_stmt[write_idx] = stmt[write_idx];
    }
    read_idx = start_second;
    while (stmt[read_idx] != '\0')
    {
        new_stmt[write_idx] = stmt[read_idx];
        write_idx++;
        read_idx++;
    }
    free(*stmt_ptr);
    *stmt_ptr = new_stmt;
}
*/


void fix_stmt(char ** stmt)
{
    trim_stmt(stmt);
    //rm_dupl_spaces(stmt);
}


void get_text(char * filename)
{
    init_symbol_table();
    init_stmts();
    //stmts = (char **)malloc(sizeof(char *)*256);
    FILE * in_file = fopen(filename, "r");
    int line_num = 1;
    char ** err_messages = (char **)malloc(sizeof(char *)*256);
    while (!feof(in_file))
    {
        char * stmt_str = (char *)malloc(sizeof(char)*MAX_STMT_LENGTH);
        fgets(stmt_str, MAX_STMT_LENGTH, in_file);
        //char * temp_msg = (char *)malloc(sizeof(char)*256);
        statement * stmt_ptr = validate_stmt(stmt_str, line_num);
        if (stmt_ptr)
        {
            if (stmt_ptr->type == ERROR)
            {
                errors_found = 1;
                print_err(stmt_ptr);
                free(stmt_str);
            }
            else if (stmt_ptr)
            {
                add_stmt(stmt_ptr);
            }
            free(stmt_ptr);
        }
        line_num++;
    }
}


