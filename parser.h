#ifndef PARSER
#define PARSER

#include <iostream>
#include <string>
#include <vector>

#include "lexer.h"

enum InstructionType
{
    BLOCK = 1000, 
    ASSIGN,
    OUTPUT,
    CJMP,
    JMP
};

struct InstructionNode;

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

struct ForStmtNode
{
    InstructionNode* instruction;
    string incrementVal;
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
        int num1Index;
        int num2Index;
        Lexer::Token op;
        bool elseStmt = false;
        bool autoFalse = true;
        
        InstructionNode* target;
        InstructionNode* elseTarget;
    } cjmp;
    struct 
    {
        int incrementVal;
        int varIndex; 

        InstructionNode* target;
    } jmp;

    InstructionNode* next = NULL;
    
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
        // static vector<struct InstructionNode *> instructions;
        InstructionNode* program();
        InstructionNode* program_body();
        InstructionNode* stmt_list();
        InstructionNode* stmt();
        InstructionNode* print_stmt();
        InstructionNode* assign_stmt();
        InstructionNode* if_stmt();
        InstructionNode* else_stmt();
        InstructionNode* for_loop();
        InstructionNode* print_line();
        AssignStmtNode arithmetic();
        AssignStmtNode condition();
        InstructionNode* body();
        ForStmtNode for_stmt();
        string primary();
        Lexer::Token op();
        Lexer::Token relop();
};

#endif PARSER
