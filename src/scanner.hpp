#ifndef MPLI_SCANNER_HPP_
#define MPLI_SCANNER_HPP_

#include "token.hpp"
#include <string>
#include <fstream>

#define MPL_KEYWORDS { "var", "for", "end", "in", "do", "read", "print", "int", "string", "bool", "assert" }

namespace mpli {

/*
 * Scanner that does lexical analysis for input character stream.
 */
class Scanner {
private:
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
