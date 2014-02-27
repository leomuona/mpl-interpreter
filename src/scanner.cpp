#include "scanner.hpp"
#include <string>

namespace mpli {

char Scanner::_readChar()
{
    return '0';
}

Token Scanner::nextToken()
{
    return Token(0, std::string());
}

} // namespace mpli
