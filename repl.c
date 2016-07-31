#include <stdio.h>
#include <stdlib.h>

#include "mpc.h"
#include "lval.h"
#include "eval.h"

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

int main(int argc, char** argv){
    mpc_parser_t* Number = mpc_new("number");
    mpc_parser_t* Operator = mpc_new("operator");
    mpc_parser_t* Expr = mpc_new("expr");
    mpc_parser_t* Slisp = mpc_new("slisp");

    mpca_lang(MPCA_LANG_DEFAULT,
    "\
    number      :   /-?[0-9]+/ ; \
    operator    :   '+' | '-' | '*' | '/' | '%' | '^'; \
    expr        :   <number> | '(' <operator> <expr>+ ')'; \
    slisp       :   /^/ <operator> <expr>+ /$/; \
    ", Number, Operator, Expr, Slisp);
    
    puts("SLISP Version 0.0.0.1");
    puts("Press Ctrl+c to Exit\n");

    while(1){
        char* input = readline("slisp> ");
        add_history(input);
        
        mpc_result_t r;
        if(mpc_parse("<stdin>", input, Slisp, &r))
        {
            lval result = eval(r.output);
            lval_println(result);
            mpc_ast_delete(r.output);
        } else {
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }
        free(input);
    }

    mpc_cleanup(4, Number, Operator, Expr, Slisp);
    return 0;
}