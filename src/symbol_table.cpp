#include "symbol_table.hpp"

namespace mpli {

void SymbolTable::push(std::string key, Symbol symbol)
{
	_symbols[key] = symbol;
}

void SymbolTable::pop(std::string key)
{
	_symbols.erase(key);
}

Symbol SymbolTable::find(std::string key)
{
	std::map<std::string, Symbol>::iterator it;
	it = _symbols.find(key);
	if (it == _symbols.end()) {
		Symbol s;
		s.type = Symbol::UNDEFINED;
		return s;
	}
	Symbol s = (*it).second;
	return s;
}

} // namespace mpli
