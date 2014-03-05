#include "scanner.hpp"
#include <cstring>

namespace mpli {

Scanner::Scanner()
{
    _curr_state = 0;

    /* ALPHA, DIGIT, PERIOD, COMMA, COLON, SEMICOLON, BRACKET_RIGHT,
        BRACKET_LEFT, PLUS, MINUS, SLASH, BACKSLASH, EXCLAMATIONM,
        ASTERISK, AND, QUOTEM, EQUALS, SPACE, TAB, NEWLINE */

    /* constructing states table */
    /* 0: initial state */
    std::vector<StateRow> v0;
    v0.push_back(StateRow(ALPHA, 1));
    v0.push_back(StateRow(DIGIT, 2));
    v0.push_back(StateRow(PERIOD, 3));
    v0.push_back(StateRow(COLON, 4));
    v0.push_back(StateRow(SEMICOLON, 5));
    v0.push_back(StateRow(BRACKET_RIGHT, 6));
    v0.push_back(StateRow(BRACKET_LEFT, 7));
    v0.push_back(StateRow(PLUS, 8));
    v0.push_back(StateRow(MINUS, 9));
    v0.push_back(StateRow(SLASH, 10));
    v0.push_back(StateRow(BACKSLASH, 11));
    v0.push_back(StateRow(EXCLAMATIONM, 12));
    v0.push_back(StateRow(ASTERISK, 13));
    v0.push_back(StateRow(AND, 14));
    v0.push_back(StateRow(QUOTEM, 15));
    v0.push_back(StateRow(EQUALS, 16));
    _states_vec.push_back(State(0, v0));

    // TODO: set certain next-states to -1 or -2
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
