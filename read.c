#include "eval.h"

lval* lval_read_num(mpc_ast_t* t) {
    errno = 0;
    long x = strtol(t->contents, NULL, 10);
    return errno != ERANGE
        ? lval_num(x)
        : lval_err("Invalid Number!");
}

lval* lval_read_str(mpc_ast_t* t) {
    t->contents[strlen(t->contents)-1] = '\0';
    char* unescaped = malloc(strlen(t->contents+1)+1);
    strcpy(unescaped, t->contents+1);
    unescaped = mpcf_unescape(unescaped);
    lval* str = lval_str(unescaped);
    free(unescaped);
    return str;
}

lval* lval_read(mpc_ast_t* t) {
    if(strstr(t->tag, "number")) { return lval_read_num(t); }
    if(strstr(t->tag, "symbol")) { return lval_sym(t->contents); }

    lval* x = NULL;

    if(strstr(t->tag, "string")) { return lval_read_str(t); }
    if(strcmp(t->tag, ">") == 0) { x = lval_sexpr(); }
    if(strstr(t->tag, "sexpr")) { x = lval_sexpr(); }
    if(strstr(t->tag, "qexpr")) { x = lval_qexpr(); }

    for(int i = 0; i < t->children_num; i++) {
        if(strcmp(t->children[i]->contents, "(") == 0) { continue; }
        if(strcmp(t->children[i]->contents, ")") == 0) { continue; }
        if(strcmp(t->children[i]->contents, "{") == 0) { continue; }
        if(strcmp(t->children[i]->contents, "}") == 0) { continue; }
        if(strcmp(t->children[i]->tag, "regex") == 0) { continue; }

        x = lval_add(x, lval_read(t->children[i]));
    }

    return x;
}