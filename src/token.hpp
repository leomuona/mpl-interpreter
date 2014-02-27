#ifndef MPLI_TOKEN_HPP_
#define MPLI_TOKEN_HPP_

#include <string>

namespace mpli {

struct Token {
    enum TYPE { Basic, Keyword, Error };

    int type;
    std::string str;

    Token(int t, std::string s) : type(t), str(s) { }
};

} // namespace mpli
#endif // MPLI_TOKEN_HPP_
