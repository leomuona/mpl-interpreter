#ifndef MPLI_SCANNER_HPP_
#define MPLI_SCANNER_HPP_

#include "token.hpp"
#include <string>
#include <fstream>
#include <vector>

#define MPL_KEYWORDS { "var", "for", "end", "in", "do", "read", "print", "int", "string", "bool", "assert" }

namespace mpli {

/*
 * Scanner that does lexical analysis for input character stream.
 */
class Scanner {
private:
    /* chartypes for our state-machine's table-structure */
    enum CHARTYPE {ALPHA, DIGIT, PERIOD, COMMA, COLON, SEMICOLON, BRACKET_RIGHT,
        BRACKET_LEFT, PLUS, MINUS, QUESTIONM, SLASH, BACKSLASH, EXCLAMATIONM,
        ASTERISK, AND, QUOTEM, EQUALS, SPACE, TAB, NEWLINE};

    struct StateRow {
        int curr_state;
        CHARTYPE next_char;
        int next_state;
        StateRow(int cs, CHARTYPE nc, int ns) : curr_state(cs), next_char(nc), next_state(ns) { }
    };

    std::vector< std::vector<StateRow>* > _states_vec;
    int _curr_state;

    std::ifstream _input_file;

    int is_whitespace(char c);
    int is_alpha(char c);
    int is_digit(char c);

    Token run_automaton(std::string *strbuffer);
    Token create_token(std::string str);
public:
    Scanner();
    ~Scanner();
    void open_input_file(char *filename);
    Token next_token();
};

} // namespace mpli

#endif // MPLI_SCANNER_HPP_
