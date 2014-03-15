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

class SymbolTable {
private:
	std::map<std::string, Symbol> _symbols;
public:
	void push(std::string key, Symbol symbol);
	void pop(std::string key);
	Symbol find(std::string key);
};

} // namespace mpli
#endif // MPLI_SYMBOL_TABLE_HPP_
