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
    bool isIndex1 = false;
    bool isIndex2 = false;
};

struct ForStmtNode
{
    InstructionNode* instruction;
    string incrementVal;
};

struct PrimaryNode //TODO: set this as the return type for primary and then rework code accordingly
{
    string index;
    bool isIndex = false;
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
        bool isIndex = false; //*Have to implement this in rest of code but this is so that arithmetic function knows if it is adding actual num or an index

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
        //boolean to see if numbers are literal numbers or just indexes
        bool num1Literal = false;
        bool num2Literal = false;
        
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
        PrimaryNode primary();
        Lexer::Token op();
        Lexer::Token relop();
};

#endif PARSER
