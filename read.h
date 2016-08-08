#ifndef read_h
    #define read_h

    #include "lval.h"

    lval* lval_read_num(mpc_ast_t* t);
    lval* lval_read(mpc_ast_t* t);
    lval* lval_read_str(mpc_ast_t* t);
#endif