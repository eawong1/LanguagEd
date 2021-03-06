#include <iostream>
#include <string>

#include "lexer.h"
#include "parser.h"

using namespace std;

void execute(InstructionNode *instructions)
{
    InstructionNode *temp = instructions;
    // InstructionNode *temp2 = instructions;

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
        cout << "Type:  " << temp->type << endl;
        // cout << "it gets here" << endl;
        if (temp->type == ASSIGN)
        {
            Variable *var = &Parser::variableList[temp->assign.lhsIndex];
            // cout << "var: " << var->id << endl;
            // cout << "seg fault: " << var->value << endl;
            if (temp->assign.op.tokenType == NOOP)
            {
                var->value = temp->assign.rhs1;
            }
            else
            {
                // cout << "op1String: " << temp->assign.rhs1 << endl; 
                int op1 = 0;
                int op2 = 0;
                if(temp->assign.isIndex1)
                {
                    Variable op = Parser::variableList[stoi(temp->assign.rhs1)];
                    op1 = stoi(op.value);
                }
                else
                {
                    op1 = stoi(temp->assign.rhs1); //TODO: Account for IDs here so can't do h = h + 1 rn
                }
                if(temp->assign.isIndex2)
                {
                    Variable op = Parser::variableList[stoi(temp->assign.rhs2)];
                    op2 = stoi(op.value);
                }
                else
                {
                    op2 = stoi(temp->assign.rhs2);
                }

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


            // cout << "numIndex: " << numIndex << endl;
            // cout << "num2Index: " << num2Index << endl;
            // cout << "auto: " << temp->cjmp.autoFalse << endl;

            // cout << "num1: " << num << endl;
            // cout << "num2: " << num2 << endl;

            bool result = false;
            if (temp->cjmp.autoFalse)
            {
                // cout << "num: " << Parser::variableList[numIndex].value << endl;

                int num = 0;
                if(!temp->cjmp.num1Literal)
                {
                    num = stoi(Parser::variableList[numIndex].value);
                    // cout << "id: " << Parser::variableList[numIndex].id << endl;
                    // cout << "value: " << Parser::variableList[numIndex].value << endl;
                }
                else 
                {
                    num = numIndex;
                }

                int num2 = 0;
                if(!temp->cjmp.num2Literal) 
                {
                    num2 = stoi(Parser::variableList[num2Index].value); 
                }
                else
                {
                    num2 = num2Index;
                }

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
                    // cout << "equal to" << endl;
                    result = (num == num2);
                    // cout << result << endl;
                }
                else if (t.tokenType == NOT_EQUAL)
                {
                    result = (num != num2);
                }
                // cout << endl;
            }
            else
            {
                result = temp->cjmp.autoFalse;
            }

            if (!result)
            {
                if (temp->cjmp.elseStmt)
                {
                    temp = temp->cjmp.elseTarget;
                }
                else
                {
                    temp = temp->cjmp.target;
                }
            }

          
        }
        else if (temp->type == JMP)
        {
            int incrementVal = temp->jmp.incrementVal;
            int varIndex = temp->jmp.varIndex;

            string strValue = Parser::variableList[varIndex].value;
            int value = stoi(strValue);

            value = value + incrementVal;

            Parser::variableList[varIndex].value = to_string(value);

            temp = temp->jmp.target;
        }

        temp = temp->next;
    }

    InstructionNode *deleted = instructions;
    while (deleted != NULL)
    {
        InstructionNode *temp = deleted->next;
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