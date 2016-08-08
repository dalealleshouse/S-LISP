#include <stdio.h>
#include <stdlib.h>

#include "mpc.h"
#include "grammar.h"
#include "lval.h"
#include "eval.h"
#include "lenv.h"
#include "builtin.h"
#include "read.h"

/*
* Mac and Linux require functionality from the realine and editline libraries, which is default in windows
* The following functions conform to the realine and edlitline APIs so this will compile on Windows, Mac, or Linux
*/
#ifdef _WIN32
    #include <string.h>

    static char buffer[2048];

    char* readline(char* prompt){
        fputs(prompt, stdout);
        fgets(buffer, 2048, stdin);
        char* cpy = malloc(strlen(buffer) + 1);
        strcpy(cpy, buffer);
        cpy[strlen(cpy)-1] = '\0';
        return cpy;
    }

    void add_history(char* unused){}
#else
    #include <editline/readline.h>
    #include <editline/history.h>
#endif

grammar_parsers* gp;

int main(int argc, char** argv){
    gp = grammar_init();
    lenv* e = lenv_new();
    lenv_add_builtins(e);
    
    if (argc == 1) {
        puts("SLISP Version 0.0.0.1");
        puts("Press Ctrl+c to Exit\n");

        while(1){
            char* input = readline("slisp> ");
            add_history(input);
            
            mpc_result_t r;
            if(mpc_parse("<stdin>", input, gp->slisp, &r))
            {
                lval* x = lval_eval(e, lval_read(r.output));
                lval_println(x);
                lval_del(x);
                mpc_ast_delete(r.output);
            } else {
                mpc_err_print(r.error);
                mpc_err_delete(r.error);
            }
            free(input);
        }
    }

    if (argc >= 2) {
        for (int i=1; i<argc; i++) {
            lval* args = lval_add(lval_sexpr(), lval_str(argv[i]));
            lval* x = builtin_load(e, args);
            if (x->type == LVAL_ERR) { lval_println(x); }
            lval_del(x);
        }
    }

    lenv_del(e);
    grammar_free(gp);
    return 0;
}