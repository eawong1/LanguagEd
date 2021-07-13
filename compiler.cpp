#include <iostream>
#include <string>

#include "lexer.h"
#include "parser.h"

using namespace std;

void execute(InstructionNode *instructions)
{
    InstructionNode *temp = instructions;
    InstructionNode *temp2 = instructions;

    while (temp2 != NULL)
    {
        cout << "Type: " << temp2->type << endl;
        temp2 = temp2->next;
    }

    while (temp != NULL)
    {
        // cout << "it gets here" << endl;
        if (temp->type == ASSIGN)
        {
            Variable *var = &Parser::variableList[temp->assign.lhsIndex];
            // cout << "var: " << var->id << endl;
            if (temp->assign.op.tokenType == NOOP)
            {
                var->value = temp->assign.rhs1;
                // cout << "var value: " << var->value << endl;
                // cout << "array value: " << Parser::variableList[temp->assign.lhsIndex].value << endl;
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
            // cout << "index: " << temp->output.index << endl;
            cout << var.value << endl;
        }
        else if (temp->type == CJMP)
        {
            // cout << "num1: " << temp->cjmp.num1 << endl;
            //TODO: values not set in the array when num1 and num2 were created... values are set after during compilation stage. 
            // cout << "it gets here cjmp" << endl;
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
            }

        }
        else if (temp->type == JMP)
        {
        }

        if(temp != NULL)
        {
            temp = temp->next;
        }
    }

    // for (int i = 0; i < Parser::instructions.size(); i++)
    // {
    //     InstructionNode *temp = Parser::instructions[i];

    // }

    //clear vector from memory to avoid mem leak
    // for (int i = 0; i < Parser::instructions.size(); i++)
    // {
    //     delete Parser::instructions[i];
    // }
    // Parser::instructions.clear(); //to clear all the nulls from the vector

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