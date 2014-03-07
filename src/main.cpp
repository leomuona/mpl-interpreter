#include "scanner.hpp"
#include "token.hpp"

#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " FILENAME" << std::endl;
        return 1;
    }
    
    std::string filename(argv[1]);
    std::cout << "Tokens for source file " << filename << std::endl;
    
    using namespace mpli;

    Scanner scanner;
    scanner.open_input_file(filename.c_str());
    Token t = scanner.next_token();
    while (t.type != Token::ERROR && t.type != Token::END_OF_FILE) {
        std::cout << t.to_string() << std::endl;
        t = scanner.next_token();
    }
    if (t.type == Token::END_OF_FILE) {
        std::cout << "End of source file " << filename << std::endl;
    } else {
        std::cout << "Error: token '" << t.str << "' is not valid." << std::endl;
    }
    return 0;
}
