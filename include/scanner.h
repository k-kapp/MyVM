#include "types.h"
#ifndef SCANNER_H
#define SCANNER_H


extern const char * reserved_words [];
extern statement * stmts;
extern int stmt_arr_len;
extern int stmt_arr_allocd;
extern int stmt_idx;

void trim_stmt(char * stmt);
char * get_text(char * filename);

#endif // SCANNER_H
