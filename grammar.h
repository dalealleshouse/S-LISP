#ifndef grammar_h
    #define grammer_h

    #include "mpc.h"

    typedef struct {
        mpc_parser_t* number;
        mpc_parser_t* symbol;
        mpc_parser_t* sexpr;
        mpc_parser_t* expr; 
        mpc_parser_t* slisp;
    } grammar_parsers;

    grammar_parsers* grammar_init(void);
    void grammar_free(grammar_parsers* parsers);
#endif