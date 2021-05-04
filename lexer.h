#include <iostream>
#include <string>

using namespace std;

typedef enum
{
    END_OF_FILE = 0, PRINT, ID, EQUALS, STRING,
    IF, ELSE, FOR, LPARENTHESES, RPARENTHESES, TO,
    WITH, OCURLY, CCURLY, COMMA, NUM, PLUS, 
    MINUS, MULT, DIV, GREATER, LESS, GREATER_EQUAL,
    LESS_EQUAL, EQUALTO, NOT_EQUAL
} TokenType;

class Lexer
{
    private:
        // string readInput(string line);
        

    public:
        struct Token
        {
            string lexeme;
            TokenType tokenType; 
        };

        void setFile(string file);
        void openFile();
        bool readLine();
        Token getToken();
        Token ungetToken(Token token);
};