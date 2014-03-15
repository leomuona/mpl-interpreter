#ifndef MPLI_INTERPRETER_HPP_
#define MPLI_INTERPRETER_HPP_

#include "ast.hpp"
#include "symbol_table.hpp"
#include <vector>
#include <string>

namespace mpli {

class Interpreter {
	private:
		SymbolTable _symbol_table;
		std::vector<int> _int_values;
		std::vector<int> _bool_values;
		std::vector<std::string> _string_values;

		/* return value: 0 = OK, 1 = Error */
		int execute_var_init(ASTNode *node);
		int execute_insert(ASTNode *node);
		int execute_for_loop(ASTNode *node);
		int execute_read(ASTNode *node);
		int execute_print(ASTNode *node);
		int execute_assert(ASTNode *node);
	public:
		int execute(AST *ast);
};

} // namespace mpli
#endif // MPLI_INTERPRETER_HPP_
