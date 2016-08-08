#ifndef grammar_h
    #define grammar_h

    #include "mpc.h"

    typedef struct {
        mpc_parser_t* number;
        mpc_parser_t* string;
        mpc_parser_t* symbol;
        mpc_parser_t* comment;
        mpc_parser_t* sexpr;
        mpc_parser_t* qexpr;
        mpc_parser_t* expr; 
        mpc_parser_t* slisp;
    } grammar_parsers;

    grammar_parsers* grammar_init(void);
    void grammar_free(grammar_parsers* parsers);
#endif