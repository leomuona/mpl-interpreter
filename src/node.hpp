#ifndef MPLI_NODE_HPP_
#define MPLI_NODE_HPP_

#include "token.hpp"
#include <vector>

namespace mpli {

struct Node {

    enum TYPE { TOKEN, PROG, STMTS, STMT, EXPR, OPND };

    TYPE type;

    std::vector<Node*> children;

    /* only if type == TOKEN */
    Token token;
};

} // namespace mpli
#endif // MPLI_NODE_HPP_
