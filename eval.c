#include "eval.h"

lval* lval_eval_sexpr(lenv* e, lval* v) {
   for(int i = 0; i < v->count; i++) {
       v->cell[i] = lval_eval(e, v->cell[i]);
   }

   for(int i = 0; i < v->count; i++) {
       if(v->cell[i]->type == LVAL_ERR) { return lval_take(v, i); }
   }

   if(v->count == 0) { return v; }
   if(v->count == 1) { return lval_take(v, 0); }

   lval* f = lval_pop(v, 0);
   if(f->type != LVAL_FUN) {
       lval* err = lval_err("S-Expression starts with incorrect type. Got %s, Expected %s",
        ltype_name(f->type), ltype_name(LVAL_FUN));
       
       lval_del(v);
       lval_del(f);
       return err;
   }

   lval* result = f->fun(e, v);
   lval_del(f);
   return result;
}

lval* lval_eval(lenv* e, lval* v) {
    if (v->type == LVAL_SYM) {
        lval* x = lenv_get(e, v);
        lval_del(v);
        return x;
    }
    if(v->type == LVAL_SEXPR) { return lval_eval_sexpr(e, v); }
    return v; 
}