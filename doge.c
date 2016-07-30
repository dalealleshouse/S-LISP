#include "mpc.h"

void mpc_functions() {
    mpc_parser_t* Adjective = mpc_or(4,
        mpc_sym("wow"), mpc_sym("many"),
        mpc_sym("so"), mpc_sym("such")
    );

    mpc_parser_t* Noun = mpc_or(5,
        mpc_sym("lisp"), mpc_sym("language"),
        mpc_sym("book"), mpc_sym("build"),
        mpc_sym("c")
    );

    mpc_parser_t* Phrase = mpc_and(2, mpcf_strfold, Adjective, Noun, free);

    mpc_parser_t* Doge = mpc_many(mpcf_strfold, Phrase);

    mpc_delete(Doge);
}

void mpc_declarative() {
    mpc_parser_t* Adjective = mpc_new("adjective");
    mpc_parser_t* Noun = mpc_new("noun");
    mpc_parser_t* Phrase = mpc_new("phrase");
    mpc_parser_t* Doge = mpc_new("doge");

    mpca_lang(MPCA_LANG_DEFAULT,
    "\
    adjective   :   \"wow\" | \"many\" | \"so\" | \"such\";\
    noun        :   \"lisp\" | \"language\" | \"book\" | \"build\" | \"c\";\
    phrase      :   <adjective> <noun>;\
    doge        :   <phrase>*;\
    ", Adjective, Noun, Phrase, Doge);

    mpc_cleanup(4, Adjective, Noun, Phrase, Doge);
}

int main(int argc, char** argv) {
    mpc_functions();
    mpc_declarative();

    return 0;
}