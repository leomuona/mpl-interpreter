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
        QUOTEM, EQUALS, LESS_THAN ,UNDERSCORE, SPACE, TAB, NEWLINE, OTHER, NOT_ANY};

	/* structure that is used in state rows */
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
	static const int _TOKEN_SKIP_STATE = -3;
	static const int _TOKEN_ERROR_STATE = -4;

	/* the state automaton map */
    std::map<int, std::vector<StateRow> > _states_map;

	/* input file */
    std::ifstream _input_file;

	/* returns true if character is space, tab or linebreak */
    int is_whitespace(char c);
	/* returns true if character is alphabetical a-z or A-Z */
    int is_alpha(char c);
	/* returns true if character is a digit */
    int is_digit(char c);
	/* what is characters CHARTYPE */
    CHARTYPE get_char_type(char c);
	/* define Token::TYPE for string */
    Token::TYPE get_token_type(std::string str);

	/* returns next state of states map */
    int get_next_state(char next_char, int curr_state);
	/* run automaton to get Token */
    Token run_automaton(std::string *strbuffer);
    /* create token */
	Token create_token(std::string str);
    /* create error token */
	Token create_error_token(std::string str);
public:
    Scanner();
    ~Scanner();
	/* open given input file, must be called before next_token() */
    void open_input_file(const char *filename);
	/* returns next token of token stream */
    Token next_token();
};

} // namespace mpli

#endif // MPLI_SCANNER_HPP_
