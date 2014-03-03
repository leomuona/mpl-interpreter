#include "scanner.hpp"
#include <cstring>

namespace mpli {

Scanner::Scanner()
{

}

Scanner::~Scanner()
{
    if (_input_file.is_open())
        _input_file.close();
}

int Scanner::is_whitespace(char c)
{
    char ws_chars[] = { ' ', '\n', '\t', 0 };
    return (strchr(ws_chars, c) != NULL);
}

int Scanner::is_alpha(char c)
{
    char alpha_chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    return (strchr(alpha_chars, c) != NULL);
}

int Scanner::is_digit(char c)
{
    char digit_chars[] = "0123456789";
    return (strchr(digit_chars, c) != NULL);
}

Token Scanner::run_automaton(std::string *strbuffer)
{
    char curr_c = 0;
    _input_file.get(curr_c);
    /* get rid of whitespace */
    while(is_whitespace(curr_c) && strbuffer->size() == 0 && _input_file.good()) {
        _input_file.get(curr_c);
    }
    
    if (!_input_file.good()) {
        return create_token(*strbuffer);
    }

    // TODO: filter comments away

     
 
    return create_token(*strbuffer);
}

Token Scanner::create_token(std::string str)
{
    if (str.size() == 0) {
        if (_input_file.eof())
            return Token(Token::EndOfFile, str);
        return Token(Token::Error, str);
    }

    // TODO construct token from given string
    return Token(0, str);
}

void Scanner::open_input_file(char *filename)
{
    if (_input_file.is_open())
        _input_file.close();
    _input_file.open(filename);     
}

Token Scanner::next_token()
{
    _curr_state = 1;
    if (_input_file.good()) {
        /* run automaton with string buffer */
        std::string *strbuffer;
        strbuffer = new std::string();
        Token t = run_automaton(strbuffer);
        delete strbuffer;
        return t;
    } else {
        /* create error token */
        return create_token(std::string());
    } 
}

} // namespace mpli
