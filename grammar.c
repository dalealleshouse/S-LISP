#include "grammar.h"

grammar_parsers* grammar_init(void) {
    grammar_parsers* gp = malloc(sizeof(grammar_parsers));

    gp->number = mpc_new("number");
    gp->symbol = mpc_new("symbol");
    gp->sexpr = mpc_new("sexpr");
    gp->qexpr = mpc_new("qexpr");
    gp->expr = mpc_new("expr");
    gp->slisp = mpc_new("slisp");
    
    mpca_lang(MPCA_LANG_DEFAULT,
    "                                                                       \
    number      :   /-?[0-9]+/ ;                                            \
    symbol      :   \"list\" | \"head\" | \"tail\" | \"join\" | \"eval\" |  \
                    '+' | '-' | '*' | '/' | '%' | '^';                      \
    sexpr       :   '(' <expr>* ')';                                        \
    qexpr       :   '{' <expr>* '}';                                        \
    expr        :   <number> | <symbol> | <sexpr> | <qexpr>;                \
    slisp       :   /^/ <expr>* /$/;                                        \
    ", gp->number, gp->symbol, gp->sexpr, gp->qexpr, gp->expr, gp->slisp);

    return gp;
}

void grammar_free(grammar_parsers* gp) {
    mpc_cleanup(6, gp->number, gp->symbol, gp->sexpr, gp->qexpr, gp->expr, gp->slisp);
    free(gp);
}