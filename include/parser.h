#ifndef PARSER_H
#define PARSER_H

extern int errors_found;

void execute_push(statement * stmt);
void execute_pop(statement * stmt);
void execute_add();
void execute_ifeq(statement * stmt);
void execute_jmp(statement * stmt);
void execute_print();
void execute_dup();
void execute_mul();
void execute_stmt(statement * stmt);
void execute_program();


#endif // PARSER_H
