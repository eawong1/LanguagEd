#include <iostream>
#include <string>

#include "lexer.h"
#include "parser.h"

using namespace std;

void execute()
{
    for (InstructionNode *temp : Parser::instructions)
    {
        if (temp->type == ASSIGN)
        {
            Variable *var = &Parser::variableList[temp->assign.lhsIndex];
            // cout << "var: " << var->id << endl;
            if (temp->assign.op.tokenType == NOOP)
            {
                var->value = temp->assign.rhs1;
            }
            else
            {
                // cout << "op1: " << temp->assign.rhs1 << endl;
                int op1 = stoi(temp->assign.rhs1);
                int op2 = stoi(temp->assign.rhs2);
                Lexer::Token optr = temp->assign.op;
                int ans = 0;
                if (optr.tokenType == PLUS)
                {
                    ans = op1 + op2;
                }
                else if (optr.tokenType == MINUS)
                {
                    ans = op1 - op2;
                }
                else if (optr.tokenType == MULT)
                {
                    ans = op1 * op2;
                }
                else if (optr.tokenType == DIV)
                {
                    ans = op1 / op2;
                }
                var->value = to_string(ans);
            }
        }
        else if (temp->type == OUTPUT)
        {
            Variable var = Parser::variableList[temp->output.index];
            cout << var.value << endl;
        }
        else if (temp->type == CJMP)
        {
        }
        else if (temp->type == JMP)
        {
        }
    }

    //clear vector from memory to avoid mem leak
    for (int i = 0; i < Parser::instructions.size(); i++)
    {
        delete Parser::instructions[i];
    }
    Parser::instructions.clear(); //to clear all the nulls from the vector
}

int main(int argc, char **argv)
{
    string file = argv[1];

    Lexer lexer;

    lexer.setFile(file); //sets the file name up
    lexer.openFile();    //opens the file and keeps it open
    // lexer.readLine(); //reads the first line of the file

    Parser parser;
    parser.program();

    execute();
}