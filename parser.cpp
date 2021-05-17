#include <iostream>
#include <string>

#include "lexer.h"
#include "parser.h"

using namespace std;

void parserDebug(string method)
{
    cout << "it gets here: " << method << endl;
}

void Parser::syntax_error()
{
    cout << "SYNTAX ERROR" << endl;
}

Lexer::Token Parser::peek()
{
    Lexer lexer;
    Lexer::Token t = lexer.getToken();
    lexer.ungetToken(t);

    return t;
}

void Parser::program()
{
    parserDebug("program");

    program_body();
}

void Parser::program_body()
{
    parserDebug("program_body");

    stmt_list();
}

void Parser::stmt_list()
{
    parserDebug("stmt_list");

    stmt();

    Lexer::Token t = peek();
    if (t.tokenType == PRINT || t.tokenType == ID || t.tokenType == IF || t.tokenType == FOR)
    {
        stmt_list();
    }
}

void Parser::stmt()
{
    parserDebug("stmt");

    Lexer::Token t = peek();
    if (t.tokenType == PRINT)
    {
        print_stmt();
    }
    else if (t.tokenType == ID)
    {
        assign_stmt();
    }
    else if (t.tokenType == IF)
    {
        if_stmt();
    }
    else if (t.tokenType == FOR)
    {
        for_loop();
    }
}

void Parser::print_stmt()
{
    parserDebug("print_stmt");

    Lexer lexer;
    Lexer::Token t = lexer.getToken();

    if (t.tokenType != PRINT)
    {
        syntax_error();
    }
    else
    {
        print_line();
    }
}

void Parser::assign_stmt()
{
    parserDebug("assign_stmt");

    Lexer lexer;
    Lexer::Token t = lexer.getToken();

    if (t.tokenType != ID)
    {
        syntax_error();
    }

    t = lexer.getToken();
    if (t.tokenType != EQUALS)
    {
        syntax_error();
    }

    t = lexer.getToken();
    if (t.tokenType != STRING)
    {
        lexer.ungetToken(t);
        arithmetic();
    }
}

void Parser::if_stmt()
{
    parserDebug("if_stmt");

    Lexer lexer;
    Lexer::Token t = lexer.getToken();

    if (t.tokenType != IF)
    {
        syntax_error();
    }

    condition();
    body();

    t = peek();
    if (t.tokenType == ELSE)
    {
        else_stmt();
    }
}

void Parser::else_stmt()
{
    parserDebug("else_stmt");

    Lexer lexer;
    Lexer::Token t = lexer.getToken();

    if (t.tokenType != ELSE)
    {
        syntax_error();
    }

    body();
}

void Parser::for_loop()
{
    parserDebug("for_loop");

    Lexer lexer;
    Lexer::Token t = lexer.getToken();

    if (t.tokenType != FOR)
    {
        syntax_error();
    }

    t = lexer.getToken();
    if (t.tokenType != LPARENTHESES)
    {
        if (t.tokenType != ID && t.tokenType != NUM)
        {
            syntax_error();
        }
        else if (t.tokenType == ID || t.tokenType == NUM)
        {
            lexer.ungetToken(t);
            primary();

            t = lexer.getToken();

            if (t.tokenType != TO)
            {
                syntax_error();
            }

            primary();

            t = lexer.getToken();
            if (t.tokenType != WITH)
            {
                syntax_error();
            }

            t = lexer.getToken();
            if (t.tokenType != ID)
            {
                syntax_error();
            }

            body();
        }
    }
    else
    {
        for_stmt();

        t = lexer.getToken();
        if (t.tokenType != RPARENTHESES)
        {
            syntax_error();
        }

        body();
    }
}

void Parser::print_line()
{
    parserDebug("print_line");

    Lexer lexer;
    Lexer::Token t = lexer.getToken();

    if (t.tokenType != ID && t.tokenType != STRING)
    {
        syntax_error();
    }
}

void Parser::arithmetic()
{
    parserDebug("arithmetic");

    Lexer lexer;
    primary();

    Lexer::Token t = peek();
    if (t.tokenType == PLUS || t.tokenType == MINUS || t.tokenType == MULT || t.tokenType == DIV)
    {
        op();
        primary();
    }
}

void Parser::condition()
{
    parserDebug("condition");

    Lexer lexer;
    Lexer::Token t = lexer.getToken();
    if (t.tokenType != ID)
    {
        syntax_error();
    }

    relop();
    primary();
}

void Parser::body()
{
    parserDebug("body");

    Lexer lexer;
    Lexer::Token t = lexer.getToken();
    if (t.tokenType != OCURLY)
    {
        syntax_error();
    }

    stmt_list();

    t = lexer.getToken();
    if (t.tokenType != CCURLY)
    {
        syntax_error();
    }
}

void Parser::condition()
{
    parserDebug("condition");

    Lexer lexer;
    Lexer::Token t = lexer.getToken();

    if (t.tokenType != ID)
    {
        syntax_error();
    }

    relop();
    primary();
}

void Parser::body()
{
    parserDebug("body");

    Lexer lexer;
    Lexer::Token t = lexer.getToken();

    if (t.tokenType != OCURLY)
    {
        syntax_error();
    }

    stmt_list();

    t = lexer.getToken();
    if (t.tokenType != CCURLY)
    {
        syntax_error();
    }
}

void Parser::for_stmt()
{
    parserDebug("for_stmt");

    assign_stmt();

    Lexer lexer;
    Lexer::Token t = lexer.getToken();

    if (t.tokenType != COMMA)
    {
        syntax_error();
    }

    condition();

    t = lexer.getToken();
    if (t.tokenType != COMMA)
    {
        syntax_error();
    }

    t = lexer.getToken();
    if (t.tokenType != NUM)
    {
        syntax_error();
    }
}

void Parser::primary()
{
    parserDebug("primary");

    Lexer lexer;
    Lexer::Token t = lexer.getToken();
    if (t.tokenType != ID && t.tokenType != NUM)
    {
        syntax_error();
    }
}

void Parser::op()
{
    parserDebug("op");

    Lexer lexer;
    Lexer::Token t = lexer.getToken();
    if (t.tokenType != PLUS && t.tokenType != MINUS && t.tokenType != MULT && t.tokenType != DIV)
    {
        syntax_error();
    }
}

void Parser::relop()
{
    parserDebug("relop");

    Lexer lexer;
    Lexer::Token t = lexer.getToken();
    if (t.tokenType == GREATER && t.tokenType == LESS && t.tokenType == GREATER_EQUAL && t.tokenType == LESS_EQUAL && t.tokenType == EQUALTO && t.tokenType == NOT_EQUAL)
    {
        syntax_error();
    }
}
