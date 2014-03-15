#include "scanner.hpp"
#include "parser.hpp"
#include "ast.hpp"

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
	if (parser.number_of_errors() > 0) {
		std::cout << "Parser found errors. Exiting." << std::endl;
		return 0;
	}
	parser.debug_print();
	AST ast;
	parser.create_ast(&ast);
	ast.debug_print();

    std::cout << "Done." << std::endl;
    return 0;
}
