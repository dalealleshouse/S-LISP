#include "eval.h"

lval* lval_eval(lval* v) {
    if(v->type == LVAL_SEXPR) {
        return lval_eval_sexpr(v);
    }
    return v; 
}

lval* lval_eval_sexpr(lval* v) {
   for(int i = 0; i < v->count; i++) {
       v->cell[i] = lval_eval(v->cell[i]);
   }

   for(int i = 0; i < v->count; i++) {
       if(v->cell[i]->type == LVAL_ERR) {
           return lval_take(v, i);
       }
   }

   if(v->count == 0) {
       return v;
   }

   if(v->count == 1) {
       return lval_take(v, 0);
   }

   lval* f = lval_pop(v, 0);
   if(f->type != LVAL_SYM) {
       lval_del(f);
       lval_del(v);
       return lval_err("S-expression Does not start with symbol!");
   }

   lval* result = builtin_op(v, f->sym);
   lval_del(f);
   return result;
}

lval* builtin_op(lval* a, char* op) {
    for(int i = 0; i < a->count; i++) {
        if(a->cell[i]->type != LVAL_NUM) {
            lval_del(a);
            return lval_err("Cannot operate on non-number!");
        }
    }

    lval* x = lval_pop(a, 0);

    if((strcmp(op, "-") == 0) && a->count == 0) {
        x->num = -x->num;
    }

    while(a->count > 0) {
        lval* y = lval_pop(a, 0);

        if(strcmp(op, "+") == 0) { x->num += y->num; }
        if(strcmp(op, "-") == 0) { x->num -= y->num; }
        if(strcmp(op, "*") == 0) { x->num *= y->num; }
        if(strcmp(op, "%") == 0) { x->num = x->num % y->num; }
        if(strcmp(op, "^") == 0) { x->num = pow(x->num, y->num); }
        
        if(strcmp(op, "/") == 0) { 
            if(y->num == 0) {
                lval_del(x);
                lval_del(y);
                x = lval_err("Division By Zero!");
                break;
            }
            x->num /= y->num;
        }

        lval_del(y);
    }

    lval_del(a);
    return x;
}