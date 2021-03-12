#include <iostream>
#include <string>
#include <vector>

#include "lexer.h"

using namespace std;

vector<string> inputLine;

string Lexer::readInput(string line)
{
    string word = "";
    
    int i = 0;
    while(line[i] != ' ')
    {
        word = word + line[i];
        i++;
    }

    return word;
}

Lexer::Token Lexer::getToken()
{
    
}