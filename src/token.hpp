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

    std::string to_string() {
        const char* type_strs[] = { "IDENTIFIER", "INTEGER", "STRING", "BOOLEAN", "OPERATOR", "SYMBOLIC", "KEYWORD", "END_OF_FILE", "ERROR" };
        std::string type_str(type_strs[type]);
        return "type: " + type_str + " str: " + str;
    }
};

} // namespace mpli
#endif // MPLI_TOKEN_HPP_
