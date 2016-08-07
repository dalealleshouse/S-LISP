#ifndef env_h
    #define env_h
    
    #include "lval.h"
    #include "eval.h"

    struct lenv {
        int count;
        char** syms;
        lval** vals;
    };

    lenv* lenv_new(void);
    lval* lenv_get(lenv* e, lval* k);
    void lenv_del(lenv* e);
    void lenv_put(lenv* e, lval* k, lval* v);
#endif