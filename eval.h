#ifndef eval_h
    #define eval_h
    
    #include <stdio.h>
    #include "mpc.h"
    #include "lval.h"

    lval* lval_eval(lval* v);
    lval* lval_eval_sexpr(lval* v);
    lval* builtin_head(lval* a);
    lval* builtin_tail(lval* a);
    lval* builtin_list(lval* a);
    lval* builtin_eval(lval* a);
    lval* builtin_join(lval* a);
    lval* lval_join(lval* x, lval* y);
    lval* builtin(lval* a, char* func);
    lval* builtin_op(lval* a, char* op);
#endif