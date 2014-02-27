#ifndef MPLI_SCANNER_HPP_
#define MPLI_SCANNER_HPP_

#include "token.hpp"

namespace mpli {

/*
 * Scanner that does lexical analysis for input character stream.
 */
class Scanner {
private:
    char _readChar();
public:
    Token nextToken();
};

} // namespace mpli

#endif // MPLI_SCANNER_HPP_
