#include "eval.h"

#define LASSERT(args, cond, fmt, ...) \
    if(!(cond)) { lval* err = lval_err(fmt, ##__VA_ARGS__); lval_del(args); return err; }

lval* lval_eval(lenv* e, lval* v) {
    if (v->type == LVAL_SYM) {
        lval* x = lenv_get(e, v);
        lval_del(v);
        return x;
    }
    if(v->type == LVAL_SEXPR) { return lval_eval_sexpr(e, v); }
    return v; 
}

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
       lval_del(v);
       lval_del(f);
       return lval_err("First element is not a function!");
   }

   lval* result = f->fun(e, v);
   lval_del(f);
   return result;
}

lval* builtin_head(lenv* e, lval* a) {
    LASSERT(a, a->count == 1, 
        "Function 'head' passed too many arguments! Got %i, Expected %i",
        a->count, 1);
    LASSERT(a, a->cell[0]->type == LVAL_QEXPR,
        "Function 'head' passed incorrect type for argument 0! Got %s, Expected %s",
        ltype_name(a->cell[0]->type), ltype_name(LVAL_QEXPR));
    LASSERT(a, a->cell[0]->count != 0, "Function 'head' passed {}!"); 

    lval* v = lval_take(a, 0);
    while(v->count > 1) { lval_del(lval_pop(v, 1)); }
    return v;
}

lval* builtin_tail(lenv* e, lval* a) {
    LASSERT(a, a->count == 1, "Function 'tail' passed too many arguments!");
    LASSERT(a, a->cell[0]->type == LVAL_QEXPR, "Function 'tail' passed incorrect type!");
    LASSERT(a, a->cell[0]->count != 0, "Function 'tail' passed {}!"); 

    lval* v = lval_take(a, 0);
    lval_del(lval_pop(v, 0));
    return v;
}

lval* builtin_list(lenv* e, lval* a) {
    a->type = LVAL_QEXPR;
    return a;
}

lval* builtin_eval(lenv* e, lval* a) {
    LASSERT(a, a->count == 1, "Function 'eval' has too many arguments!");
    LASSERT(a, a->cell[0]->type == LVAL_QEXPR, "Function 'eval' passed incorrect type!");

    lval* x = lval_take(a, 0);
    x->type = LVAL_SEXPR;
    return lval_eval(e, x);
}

lval* builtin_join(lenv* e, lval* a) {
    for(int i = 0; i < a->count; i++) {
        LASSERT(a, a->cell[i]->type == LVAL_QEXPR, "Function 'join' passed incorrect type!");
    }

    lval* x = lval_pop(a, 0);

    while(a->count) { x = lval_join(x, lval_pop(a, 0)); }

    lval_del(a);
    return x;
}

lval* builtin(lenv* e, lval* a, char* func) {
    if(strcmp("list", func) == 0) { return builtin_list(e, a); }
    if(strcmp("head", func) == 0) { return builtin_head(e, a); }
    if(strcmp("tail", func) == 0) { return builtin_tail(e, a); }
    if(strcmp("join", func) == 0) { return builtin_join(e, a); }
    if(strcmp("eval", func) == 0) { return builtin_eval(e, a); }
    if(strstr("+-/*%^", func)) { return builtin_op(e, a, func); }
    lval_del(a);
    return lval_err("Unknown Function!");
}

lval* builtin_add(lenv* e, lval* a) { return builtin_op(e, a, "+"); }
lval* builtin_sub(lenv* e, lval* a) { return builtin_op(e, a, "-"); }
lval* builtin_mul(lenv* e, lval* a) { return builtin_op(e, a, "*"); }
lval* builtin_div(lenv* e, lval* a) { return builtin_op(e, a, "/"); }
lval* builtin_mod(lenv* e, lval* a) { return builtin_op(e, a, "%"); }
lval* builtin_pow(lenv* e, lval* a) { return builtin_op(e, a, "^"); }

lval* builtin_op(lenv* e, lval* a, char* op) {
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

lval* builtin_def(lenv* e, lval* a) {
    LASSERT(a, a->cell[0]->type == LVAL_QEXPR, "Function 'def' passed incorrect type!");

    lval* syms = a->cell[0];

    for (int i = 0; i < syms->count; i++) {
        LASSERT(a, syms->cell[i]->type == LVAL_SYM, "Function 'def' cannot define non-symbol!");
    }

    LASSERT(a, syms->count == a->count - 1, "Function 'def' cannot define incorrect number of values to symbols!");

    for (int i = 0; i < syms->count; i++) {
        lenv_put(e, syms->cell[i], a->cell[i + 1]);
    }

    lval_del(a);
    return lval_sexpr();
}

lval* lval_read_num(mpc_ast_t* t) {
    errno = 0;
    long x = strtol(t->contents, NULL, 10);
    return errno != ERANGE
        ? lval_num(x)
        : lval_err("invalid number");
}

lval* lval_read(mpc_ast_t* t) {
    if(strstr(t->tag, "number")) { return lval_read_num(t); }
    if(strstr(t->tag, "symbol")) { return lval_sym(t->contents); }

    lval* x = NULL;

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