#ifndef PARSER
#define PARSER

#include <iostream>
#include <string>

#include "lexer.h"

class Parser
{
    private:
        void syntax_error();
        void varNotFound(string var);
        Lexer::Token peek();
    public:
        void program();
        void program_body();
        void stmt_list();
        void stmt();
        void print_stmt();
        void assign_stmt();
        void if_stmt();
        void else_stmt();
        void for_loop();
        void print_line();
        string arithmetic();
        void condition();
        void body();
        void for_stmt();
        string primary();
        Lexer::Token op();
        void relop();
};

#endif PARSER
