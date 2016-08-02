#ifndef eval_h
    #define eval_h
    
    #include <stdio.h>
    #include "mpc.h"
    #include "lval.h"

    lval* lval_eval(lval* v);
    lval* lval_eval_sexpr(lval* v);
    lval* builtin_op(lval* a, char* op);
#endif