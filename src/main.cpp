#include "scanner.hpp"
#include "parser.hpp"

#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " FILENAME" << std::endl;
        return 1;
    }
    
    std::string filename(argv[1]);
    std::cout << "Running mpl-interpreter for source file " << filename << std::endl;
    
    using namespace mpli;

    Scanner scanner;
    scanner.open_input_file(filename.c_str());
    Parser parser;
    parser.set_scanner(&scanner);
    parser.start();

    std::cout << "Done." << std::endl;
    return 0;
}
