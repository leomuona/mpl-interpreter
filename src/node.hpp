#ifndef MPLI_NODE_HPP_
#define MPLI_NODE_HPP_

#include "token.hpp"
#include <vector>
#include <string>

namespace mpli {

/*
 * Node structure for parse tree. Uses also Token struct.
 */
struct Node {

    enum TYPE { TOKEN, PROG, STMTS, STMT, EXPR, OPND };

    TYPE type;

    std::vector<Node*> children;

    /* only if type == TOKEN */
    Token token;

	std::string type_str()
	{
		const char* types[] = { "TOKEN", "PROG", "STMTS", "STMT", "EXPR", "OPND" };
		return std::string(types[type]);
	}
};

} // namespace mpli
#endif // MPLI_NODE_HPP_
