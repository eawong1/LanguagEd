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


// string Lexer::readInput(string line)
// {
//     string word = "";
    
//     int i = 0;
//     while(line[i] != ' ')
//     {
//         word = word + line[i];
//         i++;
//     }

//     return word;
// }

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
    if(getline(file, line))
    {
        splitted = split(line, " ");
        //stack is LIFO so have to insert in reverse order
        reverse(splitted.begin(),splitted.end());
        for(string token : splitted)
        {
            tokens.push(token);
        }
    }
    //closes the file once there is no more lines to read
    else
    {
        // cout << "no more" << endl;
        file.close();
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

    return t;
}

//TESTING PURPOSES:
int main(int argc, char *argv[])
{
    Lexer lexer;
    lexer.setFile("test.txt");
    lexer.openFile();
    lexer.readLine();
    cout << endl;
    // lexer.readLine();
    // lexer.readLine();

    while(!tokens.empty())
    {
        cout << tokens.top() << endl;
        tokens.pop();
    }
}