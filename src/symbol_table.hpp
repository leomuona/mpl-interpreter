#ifndef MPLI_SYMBOL_TABLE_HPP_
#define MPLI_SYMBOL_TABLE_HPP_

#include <map>
#include <string>

namespace mpli {

struct Symbol {
	enum TYPE {
		UNDEFINED,
		VARIABLE_INT,
		VARIABLE_STRING,
		VARIABLE_BOOL
	};

	TYPE type;
	/* location in interpreter's <type>_values vector */
	int location;	
};

/* 
 * Symbol table that is used by AST and Interpreter.
 */
class SymbolTable {
private:
	std::map<std::string, Symbol> _symbols;
public:
	/* push symbol into symbol table */
	void push(std::string key, Symbol symbol);
	/* remove symbol from symbol table */
	void pop(std::string key);
	/* Find symbol from symbol table.
	 * Returns a symbol with type UNDEFINED if not found.
	 */
	Symbol find(std::string key);
};

} // namespace mpli
#endif // MPLI_SYMBOL_TABLE_HPP_
