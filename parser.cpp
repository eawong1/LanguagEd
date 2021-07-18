#include <iostream>
#include <string>
#include <vector>
#include "lexer.h"
#include "parser.h"

using namespace std;

//TODO: Redo everything :(

//defining vector 
vector<Variable> Parser::variableList;

// vector<struct InstructionNode *> Parser::instructions;

void parserDebug(string method)
{
    cout << method << endl;
}

void Parser::syntax_error()
{
    cout << "SYNTAX ERROR" << endl;
}

void Parser::varNotFound(string var)
{
    cout << "ERROR: The variable " << var << " has not been defined yet." << endl;
    exit(0); //terminates the program 
}

Lexer::Token Parser::peek()
{
    Lexer lexer;
    Lexer::Token t = lexer.getToken();
    lexer.ungetToken(t);

    return t;
}

InstructionNode* Parser::program()
{
    // parserDebug("program");

    InstructionNode *instructions;
    instructions = program_body();
}

InstructionNode* Parser::program_body()
{
    // parserDebug("program_body");
    InstructionNode *instructions;
    instructions = stmt_list();
}

InstructionNode* Parser::stmt_list()
{
    // parserDebug("stmt_list");
    // cout << "it gets here" << endl;
    InstructionNode* stmtList = NULL;
    // InstructionNode* nextInst = NULL;
    stmtList = stmt();
    Lexer::Token t = peek();

    if (t.tokenType == PRINT || t.tokenType == ID || t.tokenType == IF || t.tokenType == FOR)
    {
        cout << flush; //need to flush the stream to avoid buffer overflow 
        InstructionNode *temp = stmtList;
        while(temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = stmt_list();
    }

    return stmtList;
}

InstructionNode* Parser::stmt()
{
    // parserDebug("stmt");
    InstructionNode* stmtList = NULL; 

    Lexer::Token t = peek();
    if (t.tokenType == PRINT)
    {
        stmtList = print_stmt();
    }
    else if (t.tokenType == ID)
    {
        stmtList = assign_stmt();
    }
    else if (t.tokenType == IF)
    {
        stmtList = if_stmt();
    }
    else if (t.tokenType == FOR)
    {
        for_loop();
    }

    return stmtList;
}

InstructionNode* Parser::print_stmt()
{
    // parserDebug("print_stmt");
    InstructionNode* print = NULL;

    Lexer lexer;
    Lexer::Token t = lexer.getToken();

    if (t.tokenType != PRINT)
    {
        syntax_error();
    }
    else
    {
        print = print_line();
    }
    

    return print;
}

InstructionNode* Parser::assign_stmt()
{
    // parserDebug("assign_stmt");

    Lexer lexer;
    Lexer::Token t = lexer.getToken();
    string id = "";
    InstructionNode* assignment = new InstructionNode;
    AssignStmtNode tempNode;

    // tempNode.op.tokenType = NOOP;
    
    assignment->type = ASSIGN;

    if (t.tokenType != ID)
    {
        syntax_error();
    }
    else if (t.tokenType == ID)
    {
        int varIndex = 0;
        string var = t.lexeme;
        id = var;
        bool inList = false;
        //checks to see if variable exist or not
        for (Variable variable : variableList)
        {
            if (var == variable.id)
            {
                inList = true;
                break;
            }
            varIndex++;
        }

        if (!inList)
        {
            Variable tempVar;
            tempVar.id = var;
            variableList.push_back(tempVar);
        }
        assignment->assign.lhsIndex = varIndex;
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
        tempNode = arithmetic();
        
        string num1 = tempNode.num1;
        string num2 = tempNode.num2;
        Lexer::Token op = tempNode.op;
        

        assignment->assign.rhs1 = num1;
        assignment->assign.rhs2 = num2;
        assignment->assign.op = op;
  
    }
    else if (t.tokenType == STRING)
    {
        string literal = t.lexeme.substr(1, t.lexeme.size() - 2);

        assignment->assign.rhs1 = literal;
        assignment->assign.rhs2 = "";
        assignment->assign.op.tokenType = NOOP;

    }

    // instructions.push_back(assignment);


    return assignment;
}

InstructionNode* Parser::if_stmt()
{
    // parserDebug("if_stmt");
    bool isElse = false;
    InstructionNode* bodyList = NULL;
    InstructionNode* ifCondition = new InstructionNode;
    Lexer lexer;

    Lexer::Token t = lexer.getToken();
    ifCondition->type = CJMP;

    if (t.tokenType != IF)
    {
        syntax_error();
    }

    AssignStmtNode node = condition();
   

    ifCondition->cjmp.num1Index = stoi(node.num1);
    ifCondition->cjmp.num2Index = stoi(node.num2);
    ifCondition->cjmp.op = node.op;

    bodyList = body();
    

    t = peek();
    
    InstructionNode* elseStmt = NULL;
    if (t.tokenType == ELSE)
    {
        ifCondition->cjmp.elseStmt = true;
        isElse = true;
        elseStmt = else_stmt();
        // cout << "else stmt: " << elseStmt->type << endl;

        // while (temp->next != NULL)
        // {
        //     temp = temp->next;
        // }
        InstructionNode* temp = ifCondition;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        InstructionNode* block = new InstructionNode;
        block->type = BLOCK;
        temp->next = block;
        ifCondition->cjmp.elseTarget = temp->next;

        temp = ifCondition;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        ifCondition->next = elseStmt;

        
    }
    // result = true;
    
    InstructionNode* temp = bodyList;
    while(temp->next != NULL)
    {
        temp = temp->next;
    }
    
    //block is an instruction that does nothing and is solely used as a target for the if-statement to jump to 
    InstructionNode* block = new InstructionNode;
    block->type = BLOCK;
    temp->next = block;
    ifCondition->cjmp.target = temp->next;
    
    temp = ifCondition;
    while(temp->next != NULL)
    {
        temp = temp->next;
    }
    ifCondition->next = bodyList;

    if(isElse)
    {
        
    }

    return ifCondition;
}

InstructionNode* Parser::else_stmt()
{
    // parserDebug("else_stmt");

    Lexer lexer;
    Lexer::Token t = lexer.getToken();
    InstructionNode* instruction;

    if (t.tokenType != ELSE)
    {
        syntax_error();
    }

    instruction = body();

    return instruction;
}

void Parser::for_loop()
{
    // parserDebug("for_loop");

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

        forLoop = true;
        body();
    }
}

InstructionNode* Parser::print_line()
{
    // parserDebug("print_line");

    Lexer lexer;
    Lexer::Token t = lexer.getToken();
    InstructionNode* instruction = new InstructionNode;
  
    if (t.tokenType != ID && t.tokenType != STRING)
    {
        syntax_error();
    }
    else if (t.tokenType == STRING)
    {
        string literal = t.lexeme.substr(1, t.lexeme.size() - 2); //gets rid of quotes
        Variable temp;
        temp.id = "";
        temp.value = literal;
        variableList.push_back(temp);

        int index = variableList.size() - 1;
        instruction->type = OUTPUT;
        instruction->output.index = index; 
    }
    else if (t.tokenType == ID)
    {
        string value = "";
        bool found = false;
        int index = 0;
        for (Variable var : variableList)
        {
            if (var.id == t.lexeme)
            {
                value = var.value;
                found = true;
                break;
            }
            index++;
        }
        if (!found)
        {
            varNotFound(t.lexeme);
        }
        // else if (value[0] == '"')
        // {
        //     string literal = value.substr(1, value.size() - 2); //gets rid of quotes
        //     // cout << literal << endl;
        // }
        // else
        // {
        //     // cout << value << endl;
        // }
        instruction->type = OUTPUT;
        instruction->output.index = index;
    }
    // instructions.push_back(instruction);


    return instruction;
}

//TODO: no setting num1 and num2 properly fix that 
AssignStmtNode Parser::arithmetic()
{
    // parserDebug("arithmetic");
    Lexer lexer;
    AssignStmtNode tempNode;
    tempNode.op.tokenType = NOOP;

    string num = primary();
    tempNode.num1 = num;

    Lexer::Token t = peek();
    Lexer::Token optr;

    bool num2Exist = false;
    string num2 = "";
    if (t.tokenType == PLUS || t.tokenType == MINUS || t.tokenType == MULT || t.tokenType == DIV)
    {
        optr = op();
        num2 = primary();
        num2Exist = true;

        tempNode.op = optr;
        tempNode.num2 = num2;
    }
    
    // int ans = 0;
    // if (num2Exist)
    // {
    //     int op1 = stoi(num);
    //     int op2 = stoi(num2);

    //     if (optr.tokenType == PLUS)
    //     {
    //         ans = op1 + op2;
    //     }
    //     else if (optr.tokenType == MINUS)
    //     {
    //         ans = op1 - op2;
    //     }
    //     else if (optr.tokenType == MULT)
    //     {
    //         ans = op1 * op2;
    //     }
    //     else if (optr.tokenType == DIV)
    //     {
    //         ans = op1 / op2;
    //     }

    //     num = to_string(ans);
    // }

    return tempNode;
}

AssignStmtNode Parser::condition()
{
    // parserDebug("condition");
    Lexer lexer;
    AssignStmtNode node;
    Lexer::Token t = lexer.getToken();
    int num = 0;
    int num2 = 0;

    if (t.tokenType != ID)
    {
        syntax_error();
    }
    else
    {
        string id = t.lexeme;
        for(int i = 0; i < variableList.size(); i++)
        {
            Variable var = variableList[i];
            if (id == var.id)
            {
                node.num1 = to_string(i);
                break;
            }
        }
    }

    t = relop();
    node.op = t;
    node.num2 = primary();

    return node;
}

InstructionNode* Parser::body()
{
    // parserDebug("body");
    InstructionNode* stmtList = NULL;

    Lexer lexer;
    Lexer::Token t = lexer.getToken();
    if (t.tokenType != OCURLY)
    {
        syntax_error();
    }

    stmtList = stmt_list();

    t = lexer.getToken();
    if (t.tokenType != CCURLY)
    {
        syntax_error();
    }

    return stmtList;
}

void Parser::for_stmt()
{
    // parserDebug("for_stmt");

    assign_stmt();
    int value = 0;


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

string Parser::primary()
{
    // parserDebug("primary");

    Lexer lexer;
    Lexer::Token t = lexer.getToken();
    if (t.tokenType != ID && t.tokenType != NUM)
    {
        syntax_error();
    }
    else if (t.tokenType == NUM)
    {
        return t.lexeme;
    }
    else if (t.tokenType == ID)
    {
        string varValue = "";
        string index = "";
        for(int i = 0; i < variableList.size(); i++)
        {
            Variable variable = variableList[i];
            if (variable.id == t.lexeme)
            {
                index = to_string(i);
                break;
            }
        }
        return index;
    }
}

Lexer::Token Parser::op()
{
    // parserDebug("op");

    Lexer lexer;
    Lexer::Token t = lexer.getToken();
    if (t.tokenType != PLUS && t.tokenType != MINUS && t.tokenType != MULT && t.tokenType != DIV)
    {
        syntax_error();
    }
    return t;
}

Lexer::Token Parser::relop()
{
    // parserDebug("relop");

    Lexer lexer;
    Lexer::Token t = lexer.getToken();
    if (t.tokenType != GREATER && t.tokenType != LESS && t.tokenType != GREATER_EQUAL && t.tokenType != LESS_EQUAL && t.tokenType != EQUALTO && t.tokenType != NOT_EQUAL)
    {
        syntax_error();
    }

    return t;
}
