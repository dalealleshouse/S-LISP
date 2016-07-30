#include "mpc.h"

void parser() {
    mpc_parser_t* Number = mpc_new("number");
    mpc_parser_t* Operator = mpc_new("operator");
    mpc_parser_t* Expr = mpc_new("expr");
    mpc_parser_t* Slisp = new mpc_new("slisp");

    mpca_lang(MPCA_LANG_DEFAULT,
    "\
    number      :   /-?[0-9]+/;\ 
    operator    :   '+' | '-' | '*' | '/';\ 
    expr        :   <number> | '(' <operator> <expr>+ ')';
    slisp       :   /^/ <operator> <expr>+ /$/;
    ", Number, Operator, Expr, Slisp);
    );
}

void clean_up() {
    
}