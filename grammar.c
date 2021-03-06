#include "grammar.h"

grammar_parsers* grammar_init(void) {
    grammar_parsers* gp = malloc(sizeof(grammar_parsers));

    gp->number = mpc_new("number");
    gp->string = mpc_new("string");
    gp->symbol = mpc_new("symbol");
    gp->comment = mpc_new("comment");
    gp->sexpr = mpc_new("sexpr");
    gp->qexpr = mpc_new("qexpr");
    gp->expr = mpc_new("expr");
    gp->slisp = mpc_new("slisp");
    
    mpca_lang(MPCA_LANG_DEFAULT,
    "                                                                       \
    number      :   /-?[0-9]+/ ;                                            \
    string      :   /\"(\\\\.|[^\"])*\"/ ;                                  \
    symbol      :   /[a-zA-Z0-9_+%^\\-*\\/\\\\=<>!&]+/ ;                    \
    comment     :   /;[^\\r\\n]*/ ;                                         \
    sexpr       :   '(' <expr>* ')';                                        \
    qexpr       :   '{' <expr>* '}';                                        \
    expr        :   <number> | <symbol> | <string>                          \
                    | <comment> | <sexpr> | <qexpr>;                        \
    slisp       :   /^/ <expr>* /$/;                                        \
    ", gp->number, gp->string, gp->symbol, gp->comment, gp->sexpr, gp->qexpr, gp->expr, gp->slisp);

    return gp;
}

void grammar_free(grammar_parsers* gp) {
    mpc_cleanup(8, gp->number, gp->string, gp->symbol, gp->comment, gp->sexpr, gp->qexpr, gp->expr, gp->slisp);
    free(gp);
}