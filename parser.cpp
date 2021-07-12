#include <iostream>
#include <string>
#include <vector>
#include "lexer.h"
#include "parser.h"

using namespace std;

//TODO: Redo everything :(

//defining vector 
vector<Variable> Parser::variableList;

vector<struct InstructionNode *> Parser::instructions;

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

void Parser::program()
{
    // parserDebug("program");

    program_body();
}

void Parser::program_body()
{
    // parserDebug("program_body");
    stmt_list();
}

InstructionNode* Parser::stmt_list()
{
    // parserDebug("stmt_list");
    // cout << "it gets here" << endl;
    InstructionNode* stmtList = NULL;
    stmtList = stmt();

    Lexer::Token t = peek();

    if (t.tokenType == PRINT || t.tokenType == ID || t.tokenType == IF || t.tokenType == FOR)
    {
        cout << flush; //need to flush the stream to avoid buffer overflow 
        stmtList->next = stmt_list();
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

string Parser::assign_stmt()
{
    // parserDebug("assign_stmt");

    Lexer lexer;
    Lexer::Token t = lexer.getToken();
    string id = "";
    InstructionNode* assignment = new InstructionNode;
    AssignStmtNode tempNode;

    // tempNode.op.tokenType = NOOP;
    assignment->next = NULL;
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

            assignment->assign.lhsIndex = varIndex;
            // cout << "var size: " << variableList.size() << endl;
            // cout << "var Index: " << varIndex << endl;
            // cout << "value: " << variableList[varIndex].id << endl;
        }
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
        // for (int i = 0; i < variableList.size(); i++)
        // {
        //     string variable = variableList[i].id;
            
        //     if (variable == id)
        //     {
        //         variableList[i].value = num;
                
        //     }
        // }
    }
    else if (t.tokenType == STRING)
    {
        string literal = t.lexeme.substr(1, t.lexeme.size() - 2);

        assignment->assign.rhs1 = literal;
        assignment->assign.rhs2 = "";
        assignment->assign.op.tokenType = NOOP;

        // cout << "assignment: " << assignment->assign.op << endl;
        // for (int i = 0; i < variableList.size(); i++)
        // {
        //     string variable = variableList[i].id;

        //     if (variable == id)
        //     {
        //         variableList[i].value = t.lexeme;
        //     }
        // }
    }

    instructions.push_back(assignment);

    return id;
}

void Parser::if_stmt()
{
    // parserDebug("if_stmt");

    InstructionNode* bodyList = NULL;
    Lexer lexer;
    Lexer::Token t = lexer.getToken();

    if (t.tokenType != IF)
    {
        syntax_error();
    }

    result = condition();
    // cout << "result: " << result << endl;

    bodyList = body();

    t = peek();
    // cout << "token type: " << t.tokenType << endl;

    if (t.tokenType == ELSE)
    {
        else_stmt();
    }
    result = true;

    
}

void Parser::else_stmt()
{
    // parserDebug("else_stmt");

    Lexer lexer;
    Lexer::Token t = lexer.getToken();

    if (t.tokenType != ELSE)
    {
        syntax_error();
    }

    if (result)
    {
        result = false;
    }
    else if (!result)
    {
        result = true;
    }
    body();
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
    instruction->next = NULL;

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
    instructions.push_back(instruction);

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

bool Parser::condition()
{
    // parserDebug("condition");
    Lexer lexer;
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
        for (Variable var : variableList)
        {
            if (id == var.id)
            {
                num = stoi(var.value);
                break;
            }
        }
    }

    t = relop();
    num2 = stoi(primary());

    if (t.tokenType == GREATER)
    {
        return (num > num2);
    }
    else if (t.tokenType == LESS)
    {
        return (num < num2);
    }
    else if (t.tokenType == GREATER_EQUAL)
    {
        return (num >= num2);
    }
    else if (t.tokenType == LESS_EQUAL)
    {
        return (num <= num2);
    }
    else if (t.tokenType == EQUALTO)
    {
        return (num == num2);
    }
    else if (t.tokenType == NOT_EQUAL)
    {
        return (num != num2);
    }

    return false;
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

//TODO:Finish for loop. So far all I did was get the result of the condition
void Parser::for_stmt()
{
    // parserDebug("for_stmt");

    string id = assign_stmt();
    int value = 0;

    for (Variable var : variableList)
    {
        if (var.id == id)
        {
            //if the user doesn't provide an int as the increment value it will cause an exception that spits out an error
            try
            {
                value = stoi(var.value);
            }
            catch (const std::exception &e)
            {
                cout << "Error for-loop requires an int as the increment value" << endl;
            }

            break;
        }
    }

    Lexer lexer;
    Lexer::Token t = lexer.getToken();

    if (t.tokenType != COMMA)
    {
        syntax_error();
    }

    result = condition();

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
        for (Variable variable : variableList)
        {
            if (variable.id == t.lexeme)
            {
                varValue = variable.value;
            }
        }
        return varValue;
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
