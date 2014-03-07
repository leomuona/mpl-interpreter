#ifndef MPLI_SCANNER_HPP_
#define MPLI_SCANNER_HPP_

#include "token.hpp"
#include <string>
#include <fstream>
#include <vector>
#include <map>

namespace mpli {

/*
 * Scanner that does lexical analysis for input character stream.
 */
class Scanner {
private:
    /* chartypes for our state-machine's table-structure */
    enum CHARTYPE {ALPHA, DIGIT, PERIOD, COLON, SEMICOLON, BRACKET_RIGHT,
        BRACKET_LEFT, PLUS, MINUS, SLASH, BACKSLASH, EXCLAMATIONM, ASTERISK, AND,
        QUOTEM, EQUALS, UNDERSCORE, SPACE, TAB, NEWLINE, OTHER, NOT_ANY};

    struct StateRow {
        CHARTYPE next_char;
        int next_state;
        int not_flag;
        StateRow(CHARTYPE nc, int ns) : next_char(nc), next_state(ns) { not_flag = 0; }
        StateRow(CHARTYPE nc, int ns, int nf) : next_char(nc), next_state(ns), not_flag(nf) { }
    };

    /* negative integers to define static token construction states */
    static const int _TOKEN_END_STATE = -1;
    static const int _TOKEN_BREAK_STATE = -2;
    static const int _TOKEN_ERROR_STATE = -3;

    std::map<int, std::vector<StateRow> > _states_map;

    std::ifstream _input_file;

    int is_whitespace(char c);
    int is_alpha(char c);
    int is_digit(char c);
    CHARTYPE get_char_type(char c);
    Token::TYPE get_token_type(std::string str);

    int get_next_state(char next_char, int curr_state);
    Token run_automaton(std::string *strbuffer);
    Token create_token(std::string str);
    Token create_error_token(std::string str);
public:
    Scanner();
    ~Scanner();
    void open_input_file(const char *filename);
    Token next_token();
};

} // namespace mpli

#endif // MPLI_SCANNER_HPP_
