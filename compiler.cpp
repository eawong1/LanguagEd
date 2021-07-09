#include <iostream>
#include <string>

#include "lexer.h"
#include "parser.h"

using namespace std;

void execute(struct InstructionNode* program)
{
    struct InstructionNode* temp = program;

    while(temp != NULL)
    {
        if(temp->type == ASSIGN)
        {
            
        }
        else if(temp->type == PRINT)
        {
            Variable var = Parser::variableList[temp->output.index];
            cout << var.value << endl;
        }
        else if(temp->type == CJMP)
        {

        }
        else if(temp->type == JMP)
        {
            
        }
    }
}

int main(int argc, char **argv)
{
    string file = argv[1];

    Lexer lexer;
    
    lexer.setFile(file); //sets the file name up 
    lexer.openFile();  //opens the file and keeps it open 
    // lexer.readLine(); //reads the first line of the file

    Parser parser;
    parser.program();
}