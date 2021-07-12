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
        string num1;
        string num2;
        Lexer::Token op;
        struct InstructionNode* target; 
    } cjmp;
    struct 
    {
        struct InstructionNode* target;
    } jmp;

    InstructionNode* next; 
    
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
        InstructionNode* stmt_list();
        InstructionNode* stmt();
        InstructionNode* print_stmt();
        string assign_stmt();
        void if_stmt();
        void else_stmt();
        void for_loop();
        InstructionNode* print_line();
        AssignStmtNode arithmetic();
        bool condition();
        InstructionNode* body();
        void for_stmt();
        string primary();
        Lexer::Token op();
        Lexer::Token relop();
};

#endif PARSER
