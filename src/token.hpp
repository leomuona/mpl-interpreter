#ifndef MPLI_TOKEN_HPP_
#define MPLI_TOKEN_HPP_

#include <string>

namespace mpli {

struct Token {
    /* NOTE: is there a BOOLEAN token? */
    enum TYPE { IDENTIFIER, INTEGER, STRING, BOOLEAN, OPERATOR, SYMBOLIC, KEYWORD, END_OF_FILE, ERROR };

    TYPE type;
    std::string str;

    Token(TYPE t, std::string s) : type(t), str(s) { }
};

} // namespace mpli
#endif // MPLI_TOKEN_HPP_
