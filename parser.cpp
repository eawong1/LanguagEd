#include <iostream>
#include <string>
#include <vector>
#include "lexer.h"
#include "parser.h"

using namespace std;


//TODO: Reset result after it is set to false

struct Variable
{
    string id = "";
    string value = "";
};

vector<Variable> variableList;

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

void Parser::stmt_list()
{
    // parserDebug("stmt_list");

    stmt();

    Lexer::Token t = peek();
    cout << ""; //flush out null terminator string from stack 
    if (t.tokenType == PRINT || t.tokenType == ID || t.tokenType == IF || t.tokenType == FOR)
    {
        stmt_list();
    }
}

void Parser::stmt()
{
    // parserDebug("stmt");

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
    // parserDebug("print_stmt");

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
    // parserDebug("assign_stmt");

    Lexer lexer;
    Lexer::Token t = lexer.getToken();
    string id = "";
    if (t.tokenType != ID)
    {
        syntax_error();
    }
    else if(t.tokenType == ID && result)
    {
        string var = t.lexeme;
        id = var;
        bool inList = false;
        for(Variable variable : variableList)
        {
            if(var == variable.id)
            {
                inList = true;
            }
        }

        //if variable that is trying to be accessed is not found output error
        if(!inList)
        {
            Variable tempVar;
            tempVar.id = var;
            variableList.push_back(tempVar);
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
        string num = arithmetic();

        if(result)
        {
            for (int i = 0; i < variableList.size(); i++)
            {
                string variable = variableList[i].id;

                if (variable == id)
                {
                    variableList[i].value = num;
                }
            }
        }
    }
    else if(t.tokenType == STRING && result)
    {
        for(int i = 0; i < variableList.size(); i++)
        {
            string variable = variableList[i].id;

            if(variable == id)
            {
                variableList[i].value = t.lexeme;
            }
        }
    }
    
}

void Parser::if_stmt()
{
    // parserDebug("if_stmt");

    Lexer lexer;
    Lexer::Token t = lexer.getToken();

    if (t.tokenType != IF)
    {
        syntax_error();
    }

    result = condition();
    // cout << "result: " << result << endl;

    body();

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

    if(result)
    {
        result = false;
    }
    else if(!result)
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

        body();
    }
}

void Parser::print_line()
{
    // parserDebug("print_line");

    Lexer lexer;
    Lexer::Token t = lexer.getToken();

    if (t.tokenType != ID && t.tokenType != STRING)
    {
        syntax_error();
    }
    else if(t.tokenType == STRING && result)
    {
        string literal = t.lexeme.substr(1,t.lexeme.size() - 2); //gets rid of quotes
        cout << literal << endl;
    }
    else if(t.tokenType == ID && result)
    {
        string value = "";
        bool found = false;
        for(Variable var : variableList)
        {
            if(var.id == t.lexeme)
            {
                value = var.value;
                found = true;
            }
        }
        
        if(!found)
        {
            varNotFound(t.lexeme);
        }
        else if(value[0] == '"')
        {
            string literal = value.substr(1, value.size() - 2); //gets rid of quotes
            cout << literal << endl;
        }
        else
        {
            cout << value << endl;
        }
    }

}

string Parser::arithmetic()
{
    // parserDebug("arithmetic");

    Lexer lexer;
    string num = primary();

    Lexer::Token t = peek();
    Lexer::Token optr;

    bool num2Exist = false;
    string num2 = "";
    if (t.tokenType == PLUS || t.tokenType == MINUS || t.tokenType == MULT || t.tokenType == DIV)
    {
        optr = op();
        num2 = primary();
        num2Exist = true;
    }
    
    int ans = 0;
    if(num2Exist)
    {
        int op1 = stoi(num);
        int op2 = stoi(num2);

        if(optr.tokenType == PLUS)
        {
            ans = op1 + op2;
        }
        else if(optr.tokenType == MINUS)
        {
            ans = op1 - op2;
        }
        else if(optr.tokenType == MULT)
        {
            ans = op1 * op2;
        }
        else if(optr.tokenType == DIV)
        {
            ans = op1/op2;
        }

        num = to_string(ans);
    }

    return num; 
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
        for(Variable var : variableList)
        {
            if(id == var.id)
            {
                num = stoi(var.value);
                break;
            }
        }
    }

    t = relop();
    num2 = stoi(primary());

    if(t.tokenType == GREATER)
    {
        return (num > num2);
    }
    else if(t.tokenType == LESS)
    {
        return (num < num2);
    }
    else if(t.tokenType == GREATER_EQUAL)
    {
        return (num >= num2);
    }
    else if(t.tokenType == LESS_EQUAL)
    {
        return (num <= num2);
    }
    else if(t.tokenType == EQUALTO)
    {
        return (num == num2);
    }
    else if(t.tokenType == NOT_EQUAL)
    {
        return (num != num2);
    }

    return false; 



}

void Parser::body()
{
    // parserDebug("body");

    Lexer lexer;
    Lexer::Token t = lexer.getToken();
    if (t.tokenType != OCURLY)
    {
        syntax_error();
    }

    //! This could be causing an error
    stmt_list();

    t = lexer.getToken();
    if (t.tokenType != CCURLY)
    {
        syntax_error();
    }
}

void Parser::for_stmt()
{
    // parserDebug("for_stmt");

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

string Parser::primary()
{
    // parserDebug("primary");

    Lexer lexer;
    Lexer::Token t = lexer.getToken();
    if (t.tokenType != ID && t.tokenType != NUM)
    {
        syntax_error();
    }
    else if(t.tokenType == NUM)
    {
        return t.lexeme;
    }
    else if(t.tokenType == ID)
    {
        string varValue = "";
        for(Variable variable : variableList)
        {
            if(variable.id == t.lexeme)
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
