#include <iostream>
#include <string>

#include "lexer.h"
#include "parser.h"

using namespace std;

void execute(InstructionNode *instructions)
{
    InstructionNode *temp = instructions;
    InstructionNode *temp2 = instructions;
    
    // while(temp2 != NULL)
    // {
    //     cout << "Type: " << temp2->type << endl;
    //     if(temp2->type == CJMP)
    //     {
    //         cout << "Jmp type: " << temp2->cjmp.target->type << endl;
    //     }
    //     temp2 = temp2->next;
    // }

    while (temp != NULL)
    {
        bool jumped = false;
        // cout << "it gets here" << endl;
        if (temp->type == ASSIGN)
        {
            Variable *var = &Parser::variableList[temp->assign.lhsIndex];
            if (temp->assign.op.tokenType == NOOP)
            {
                var->value = temp->assign.rhs1; 
            }
            else
            {
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
          
            int numIndex = temp->cjmp.num1Index;
            int num2Index = temp->cjmp.num2Index;
            Lexer::Token t = temp->cjmp.op;

            int num = stoi(Parser::variableList[numIndex].value);
            int num2 = stoi(Parser::variableList[num2Index].value);


            bool result = false;
            if (t.tokenType == GREATER)
            {
                result = (num > num2);
            }
            else if (t.tokenType == LESS)
            {
                result = (num < num2);
            }
            else if (t.tokenType == GREATER_EQUAL)
            {
                result = (num >= num2);
            }
            else if (t.tokenType == LESS_EQUAL)
            {
                result = (num <= num2);
            }
            else if (t.tokenType == EQUALTO)
            {
                result = (num == num2);
            }
            else if (t.tokenType == NOT_EQUAL)
            {
                result = (num != num2);
            }
            
            if(!result)
            {
                temp = temp->cjmp.target;
                jumped = true;
            }

        }
        else if (temp->type == JMP)
        {
        }

        if(temp != NULL && !jumped)
        {
        }
        temp = temp->next;
    }


    InstructionNode* deleted = instructions;
    while(deleted != NULL)
    {
        InstructionNode* temp = deleted->next;
        delete deleted;
        deleted = temp;
    }
}

int main(int argc, char **argv)
{
    string file = argv[1];

    Lexer lexer;

    lexer.setFile(file); //sets the file name up
    lexer.openFile();    //opens the file and keeps it open
    // lexer.readLine(); //reads the first line of the file

    Parser parser;
    InstructionNode *instructions = parser.program();

    execute(instructions);
}