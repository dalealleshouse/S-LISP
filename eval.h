#ifndef eval_h
    #define eval_h
    
    #include <stdio.h>
    #include "mpc.h"
    #include "lval.h"
    #include "lenv.h"

    lval* lval_eval(lenv* e, lval* v);
    lval* lval_eval_sexpr(lenv* e, lval* v);
    lval* builtin_head(lenv* e, lval* a);
    lval* builtin_tail(lenv* e, lval* a);
    lval* builtin_list(lenv* e, lval* a);
    lval* builtin_eval(lenv* e, lval* a);
    lval* builtin_join(lenv* e, lval* a);
    lval* lval_join(lval* x, lval* y);
    lval* builtin(lenv* e, lval* a, char* func);
    lval* builtin_op(lenv* e, lval* a, char* op);
    lval* builtin_add(lenv* e, lval* a);
    lval* builtin_sub(lenv* e, lval* a);
    lval* builtin_mul(lenv* e, lval* a);
    lval* builtin_div(lenv* e, lval* a);
    lval* builtin_mod(lenv* e, lval* a);
    lval* builtin_pow(lenv* e, lval* a);
#endif