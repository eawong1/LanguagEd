#ifndef PARSER
#define PARSER

#include <iostream>
#include <string>
#include <vector>

#include "lexer.h"

enum InstructionType
{
    ASSIGN,
    OUTPUT,
    CJMP,
    JMP
};

struct Variable
{
    string id = "";
    string value = "";
};

struct AssignStmtNode
{
    string num1;
    string num2;
    Lexer::Token op;
};

struct InstructionNode
{
    InstructionType type;
    struct 
    {
        int lhsIndex;
        string rhs1;
        string rhs2;
        Lexer::Token op;

    } assign;
    
    struct
    {
        int index; 
    } output; 

    struct 
    {
        bool result;
        struct InstructionNode* target; 
    } cjmp;
    struct 
    {
        struct InstructionNode* target;
    } jmp;
    
};

class Parser
{
    private:
        bool result = true;
        bool forLoop = false;
        int startingNum = 0;
        int endingNum = 0;

        void syntax_error();
        void varNotFound(string var);
        Lexer::Token peek();
    public:
        static vector<Variable> variableList;
        static vector<struct InstructionNode *> instructions;
        void program();
        void program_body();
        void stmt_list();
        void stmt();
        void print_stmt();
        string assign_stmt();
        void if_stmt();
        void else_stmt();
        void for_loop();
        void print_line();
        AssignStmtNode arithmetic();
        bool condition();
        void body();
        void for_stmt();
        string primary();
        Lexer::Token op();
        Lexer::Token relop();
};

#endif PARSER
