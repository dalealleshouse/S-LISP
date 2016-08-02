#include "grammar.h"

grammar_parsers* grammar_init(void) {
    grammar_parsers* gp = malloc(sizeof(grammar_parsers));

    gp->number = mpc_new("number");
    gp->symbol = mpc_new("symbol");
    gp->sexpr = mpc_new("sexpr");
    gp->expr = mpc_new("expr");
    gp->slisp = mpc_new("slisp");
    
    mpca_lang(MPCA_LANG_DEFAULT,
    "\
    number      :   /-?[0-9]+/ ; \
    symbol      :   '+' | '-' | '*' | '/' | '%' | '^'; \
    sexpr       :   '(' <expr>* ')'; \
    expr        :   <number> | <symbol> | <sexpr>; \
    slisp       :   /^/ <expr>* /$/; \
    ", gp->number, gp->symbol, gp->sexpr, gp->expr, gp->slisp);

    return gp;
}

void grammar_free(grammar_parsers* gp) {
    mpc_cleanup(5, gp->number, gp->symbol, gp->sexpr, gp->expr, gp->slisp);
    free(gp);
}