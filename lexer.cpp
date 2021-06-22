#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stack>
#include <fstream>

#include "lexer.h"

using namespace std;

vector<string> inputLine;
stack<string> tokens;
string scriptFile;
ifstream file;


bool isNullTerminator(char check)
{
    cout << "null terminator" << endl;
    if(int(check) == 0)
    {
        return true;
    }
    return false;
}

//splits the string based on the delimited and returns a vector with the splitted values 
vector<string> split(string line, string delimiter)
{
    string word = "";
    line = line + delimiter; //adds extra delimiter to the end of the line so the because the split is checking for splits
    vector<string> split;
    //iterates through each character of the line
    for(char c : line)
    {
        // cout << "character: " << c << endl;
        if(c != ' ')
        {
            word = word + c;
        }
        //once it hits a space that means the word is complete and can be added into the vector and word is then reset
        else if(c == ' ')
        {
            split.push_back(word);
            word = "";
        }
    }

    return split;
}
//opens specified file and keeps it open so we can read line by line when we prompt it 
void Lexer::openFile()
{
    file.open(scriptFile);
}

bool Lexer::readLine()
{
    string line;
    vector<string> splitted;
    //return 'file' and file evalutes to true when previous read was successful 
    if(file.is_open() && getline(file, line))
    {
        splitted = split(line, " ");
        //stack is LIFO so have to insert in reverse order
        reverse(splitted.begin(),splitted.end());
        for(string token : splitted)
        {
            tokens.push(token);
        }
        return true;
    }
    //closes the file once there is no more lines to read
    else
    {
        file.close();
        return false;
    }
    return false;
}

void Lexer::setFile(string file)
{
    scriptFile = file;
}

Lexer::Token Lexer::getToken()
{
    Lexer::Token t;
    bool lineResult = true;

    stack<string> temp = tokens;

    //if tokens stack is empty read the next line and repopulate it 
    if(tokens.empty())
    {
        lineResult = readLine();
    }


    //if there are still more lines
    if(lineResult /*&& !isNullTerminator(tokens.top()[0])*/)
    {
        t.lexeme = tokens.top();
        // cout << "Top: " << t.lexeme << endl;

        if(t.lexeme == "print")
        {
            t.tokenType = PRINT;
        }
        else if(t.lexeme == "=")
        {
            t.tokenType = EQUALS;
        }
        else if(t.lexeme[0] == '\"')
        {
            t.tokenType = STRING;
        }
        else if(t.lexeme == "if")
        {
            t.tokenType = IF;
        }
        else if(t.lexeme == "else")
        {
            t.tokenType = ELSE;
        }
        else if(t.lexeme == "for")
        {
            t.tokenType = FOR;
        }
        else if(t.lexeme == "(")
        {
            t.tokenType = LPARENTHESES;
        }
        else if(t.lexeme == ")")
        {
            t.tokenType = RPARENTHESES;
        }
        else if(t.lexeme == "to")
        {
            t.tokenType = TO;
        }
        else if(t.lexeme == "with")
        {
            t.tokenType = WITH;
        }
        else if(t.lexeme == "{")
        {
            t.tokenType = OCURLY;
        }
        else if(t.lexeme == "}")
        {
            t.tokenType = CCURLY;
        }
        else if(t.lexeme == ",")
        {
            t.tokenType = COMMA;
        }
        //if the first char is a number, token is a number
        else if(int(t.lexeme[0]) >= 48 && int(t.lexeme[0]) <= 57)
        {
            t.tokenType = NUM;
        }
        else if(t.lexeme == "+")
        {
            t.tokenType = PLUS;
        }
        else if(t.lexeme == "-")
        {
            t.tokenType = MINUS;
        }
        else if(t.lexeme == "*")
        {
            t.tokenType = MULT;
        }
        else if(t.lexeme == "/")
        {
            t.tokenType = DIV; 
        }
        else if(t.lexeme == ">")
        {
            t.tokenType = GREATER;
        }
        else if(t.lexeme == "<")
        {
            t.tokenType = LESS;
        }
        else if(t.lexeme == ">=")
        {
            t.tokenType = GREATER_EQUAL;
        }
        else if(t.lexeme == "<=")
        {
            t.tokenType = LESS_EQUAL;
        }
        else if(t.lexeme == "==")
        {
            t.tokenType =  EQUALTO;
        }
        else if (t.lexeme == "!=")
        {
            t.tokenType = NOT_EQUAL;
        }
        //if first char is a letter then it is an ID
        //has to be last because it has to check if it is keyword before checking to see if it is an id
        else if((int(t.lexeme[0]) >= 65 && int(t.lexeme[0]) <= 90) || (int(t.lexeme[0]) >= 97 && int(t.lexeme[0]) <= 122))
        {
            t.tokenType = ID;
        }

        //pop token from stack 
        tokens.pop();

    }


    return t;
}

Lexer::Token Lexer::ungetToken(Lexer::Token token)
{
    tokens.push(token.lexeme);
    return token;
}