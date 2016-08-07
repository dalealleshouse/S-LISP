#ifndef lval_h
    #define lval_h

    #include <stdlib.h>
    #include <string.h>
    #include <stdio.h>

    #include "mpc.h"

    struct lval;
    struct lenv;
    typedef struct lval lval;
    typedef struct lenv lenv;

    enum { LVAL_ERR, LVAL_NUM, LVAL_SYM, LVAL_FUN, LVAL_SEXPR, LVAL_QEXPR };

    typedef lval*(*lbuiltin)(lenv*, lval*);

    typedef struct lval {
        int type;

        long num;
        char* err;
        char* sym;
        lbuiltin fun;
        
        int count;
        struct lval** cell;
    } lval;

    lval* lval_num(long x);
    lval* lval_err(char* m);
    lval* lval_sym(char* s);
    lval* lval_sexpr(void);
    lval* lval_qexpr(void);
    lval* lval_fun(lbuiltin func);
    lval* lval_read_num(mpc_ast_t* t);
    lval* lval_read(mpc_ast_t* t);
    lval* lval_add(lval* v, lval* x);
    lval* lval_pop(lval* v, int i);
    lval* lval_take(lval* v, int i);
    lval* lval_copy(lval* v);
    void lval_del(lval* v);
    void lval_expr_print(lval* v, char open, char close);
    void lval_print(lval* v);
    void lval_println(lval* v);
#endif