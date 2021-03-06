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
    lenv* lenv_new(void);
    void lenv_del(lenv*);
    lenv* lenv_copy(lenv*);

    enum { LVAL_ERR, LVAL_NUM, LVAL_SYM, LVAL_STR, LVAL_FUN, LVAL_SEXPR, LVAL_QEXPR };

    typedef lval*(*lbuiltin)(lenv*, lval*);

    typedef struct lval {
        int type;

        long num;
        char* err;
        char* sym;
        char* str;

        lbuiltin builtin;
        lenv* env;
        lval* formals;
        lval* body;
        
        int count;
        struct lval** cell;
    } lval;

    lval* lval_num(long x);
    lval* lval_err(char* frm, ...);
    lval* lval_sym(char* s);
    lval* lval_sexpr(void);
    lval* lval_qexpr(void);
    lval* lval_fun(lbuiltin func);
    lval* lval_lambda(lval* formals, lval* body);
    lval* lval_str(char* s);
    lval* lval_add(lval* v, lval* x);
    lval* lval_join(lval* x, lval* y);
    lval* lval_pop(lval* v, int i);
    lval* lval_take(lval* v, int i);
    lval* lval_copy(lval* v);
    void lval_del(lval* v);
    void lval_expr_print(lval* v, char open, char close);
    void lval_print(lval* v);
    void lval_println(lval* v);
    void lval_print_str(lval* v);
    int lval_eq(lval* x, lval* y);
    char* ltype_name(int t);
#endif