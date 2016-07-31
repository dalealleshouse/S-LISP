#ifndef eval_h
    #define eval_h
    
    #include <stdio.h>
    #include "mpc.h"
    #include "lval.h"

    lval eval_op(lval x, char* op, lval y);
    lval eval(mpc_ast_t* t);
#endif