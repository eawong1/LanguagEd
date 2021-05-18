#include <iostream>
#include <string>

#include "lexer.h"
#include "parser.h"

using namespace std;
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