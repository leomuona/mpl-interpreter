#ifndef MPLI_AST_HPP_
#define MPLI_AST_HPP_

#include "node.hpp"
#include "symbol_table.hpp"
#include <vector>
#include <string>

namespace mpli {

struct ASTOperator {
    enum TYPE {
        ADD,
        SUBTRACT,
        MULTIPLY,
        DIVIDE,
        LESS_THAN,
        EQUALS,
        AND,
        NOT 
    };
};

struct ASTVariable {
    enum TYPE {
        STRING,
        INTEGER,
        BOOLEAN,
        UNKNOWN
    };
};

struct ASTNode {
    enum TYPE {
        ROOT,
        INSERT,
        OPERATOR,
        UNARY_OP,
        FOR_LOOP,
        FOR_IN,
        FOR_DO,
        READ,
        PRINT,
        ASSERT,
        VAR_ID,
        VAR_INIT,
        CONSTANT
    };

    /* so, what are we having? */
    TYPE type;
    /* pointers to children*/
    std::vector<ASTNode*> children;
    
    /* value in string, if there is any */
    std::string value;

    /* if type == OPERATOR */
    ASTOperator::TYPE operator_type;
    /* if type == VAR_ID | VAR_INIT | CONSTANT */
    ASTVariable::TYPE variable_type;
};

class AST {
private:
	SymbolTable _symbol_table;

    ASTNode *_root;
	int _number_of_errors;

    void build(ASTNode *parent, Node *node);
    void build_stmt(ASTNode *parent, Node *node);
    void build_var_init(ASTNode *parent, Node *stmt_node);
    void build_insert(ASTNode *parent, Node *stmt_node);
    void build_for_loop(ASTNode *parent, Node *stmt_node);
    void build_read(ASTNode *parent, Node *stmt_node);
    void build_print(ASTNode *parent, Node *stmt_node);
    void build_assert(ASTNode *parent, Node *stmt_node);
    void build_expr(ASTNode *parent, Node *expr_node);
    void build_opnd(ASTNode *parent, Node *opnd_node);

	void report_error(std::string str);
    void delete_node_r(ASTNode *node);
	void debug_print_r(ASTNode *node, int level);
public:
    AST();
    ~AST();
    void create(Node *root);
    int number_of_errors();
	void debug_print();
};

} // namespace mpli
#endif // MPLI_AST_HPP_
