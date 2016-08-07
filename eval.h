#ifndef eval_h
    #define eval_h
    
    #include <stdio.h>
    #include "mpc.h"
    #include "lval.h"
    #include "lenv.h"

    lval* lval_eval_sexpr(lenv* e, lval* v);
    lval* lval_eval(lenv* e, lval* v);
#endif