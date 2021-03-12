#include <iostream>
#include <string>

using namespace std;

typedef enum
{
    END_OF_FILE = 0, PRINT, ID, EQUALS, STRING,
    IF, ELSE, FOR, LPARENTHESES, RPARENTHESES, TO,
    WITH, QUOTE, OCURLY, CCURLY, COMMA, NUM
} TokenType;

class Lexer
{
    private:
        string readInput(string line);

    public:
        struct Token
        {
            string lexeme;
            TokenType tokenType; 
        };

        Token getToken();
        Token ungetToken(Token token);
};