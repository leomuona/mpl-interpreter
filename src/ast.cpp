#include "ast.hpp"

#include <cstdio>

namespace mpli {

AST::AST()
{
    _root = NULL;
}

AST::~AST()
{
    if (_root) {
        delete_node_r(_root);
    }
}

void AST::delete_node_r(ASTNode *node)
{
    for(int i=(node->children.size()-1); i >= 0; --i) {
        delete_node_r(node->children[i]);
    }
    delete node;   
}

void AST::debug_print()
{
    // TODO: debug print
}

void AST::create(Node *root)
{
    _root = new ASTNode;
    _root->type = ASTNode::ROOT;
    build(_root, root);    
};

void AST::build(ASTNode *parent, Node *node)
{
    std::vector<Node*>::iterator it;
    switch (node->type) {
        case Node::PROG:
        case Node::STMTS:
            for (it = node->children.begin(); it != node->children.end(); it++) {
                build(parent, (*it));
            }
            break;
        case Node::STMT:
            build_stmt(parent, node);
            break;
        case Node::TOKEN:
            if (node->token.type == Token::SEMICOLON) {
                /* Do nothing. Correct location has been checked in parser. */
                break;
            }
        default:
            printf("Error: Invalid parse tree.\n");
    }
}

void AST::build_stmt(ASTNode *parent, Node *node)
{
    switch (node->children[0]->token.type) {
        case Token::KW_VAR:
            build_var_init(parent, node);
            break;
        case Token::IDENTIFIER:
            build_insert(parent, node);
            break;
        case Token::KW_FOR:
            build_for_loop(parent, node);
            break;
        case Token::KW_READ:
            build_read(parent, node);
            break;
        case Token::KW_PRINT:
            build_print(parent, node);
            break;
        case Token::KW_ASSERT:
            build_assert(parent, node);
            break;
        default:
            printf("Error: Invalid parse tree.\n");
    }
}

void AST::build_var_init(ASTNode *parent, Node *stmt_node)
{
    /* initialization node */
    ASTNode *var_init_node = new ASTNode;
    var_init_node->type = ASTNode::VAR_INIT;
    
    ASTVariable::TYPE var_type;
    switch (stmt_node->children[3]->token.type) {
        case Token::KW_INT:
            var_type = ASTVariable::INTEGER;
        case Token::KW_STRING:
            var_type = ASTVariable::STRING;
        case Token::KW_BOOL:
            var_type = ASTVariable::BOOLEAN;
        default:
            printf("Error: Cannot resolve variable type.");
            var_type = ASTVariable::UNKNOWN;
    }
    var_init_node->variable_type = var_type;

    /* identifier node */
    ASTNode *id_node = new ASTNode;
    id_node->type = ASTNode::VAR_ID;
    id_node->value = stmt_node->children[1]->token.str;
    id_node->variable_type = var_type;
    var_init_node->children.push_back(id_node);
   
    /* set initialization node to be children of parent */
    parent->children.push_back(var_init_node);

    if (stmt_node->children.size() == 6) {
        /* insert node */
        ASTNode *insert_node = new ASTNode;
        insert_node->type = ASTNode::INSERT;

        /* identifier node 2 */
        ASTNode *id_node2 = new ASTNode;
        id_node2->type = id_node->type;
        id_node2->value = id_node->value;
        id_node2->variable_type = id_node->variable_type;
        insert_node->children.push_back(id_node2);

        /* add expr node tree for insert node */
        build_expr(insert_node, stmt_node->children[5]);
    }

}

void AST::build_insert(ASTNode *parent, Node *stmt_node)
{
    /* insert node */
    ASTNode *insert_node = new ASTNode;
    insert_node->type = ASTNode::INSERT;

    /* identifier node */
    ASTNode *id_node = new ASTNode;
    id_node->type = ASTNode::VAR_ID;
    id_node->variable_type = ASTVariable::UNKNOWN;
    id_node->value = stmt_node->children[0]->token.str;
    insert_node->children.push_back(id_node);
   
    /* set insert node to be children of parent */ 
    parent->children.push_back(insert_node);

    /* add expr node tree for insert node */
    build_expr(insert_node, stmt_node->children[2]);
}

void AST::build_for_loop(ASTNode *parent, Node *stmt_node)
{
    /* for loop node */
    ASTNode *for_node = new ASTNode;
    for_node->type = ASTNode::FOR_LOOP;
    parent->children.push_back(for_node);

    /* for in node */
    ASTNode *in_node = new ASTNode;
    in_node->type = ASTNode::FOR_IN;
    for_node->children.push_back(in_node);
    
    /* in : identifier node */
    ASTNode *id_node = new ASTNode;
    id_node->type = ASTNode::VAR_ID;
    id_node->value = stmt_node->children[1]->token.str;
    id_node->variable_type = ASTVariable::UNKNOWN;
    in_node->children.push_back(id_node);

    /* in : left side expr */
    build_expr(in_node, stmt_node->children[3]);
    /* in : right side expr */
    build_expr(in_node, stmt_node->children[5]);

    /* for do node */
    ASTNode *do_node = new ASTNode;
    do_node->type = ASTNode::FOR_DO;
    for_node->children.push_back(do_node);

    /* do : stmts */
    build(do_node, stmt_node->children[7]);
}

void AST::build_read(ASTNode *parent, Node *stmt_node)
{
    /* read node */
    ASTNode *read_node = new ASTNode;
    read_node->type = ASTNode::READ;

    /* identifier node */
    ASTNode *id_node = new ASTNode;
    id_node->type = ASTNode::VAR_ID;
    id_node->variable_type = ASTVariable::UNKNOWN;
    id_node->value = stmt_node->children[1]->token.str;
    read_node->children.push_back(id_node);

    /* set read node to be children of parent */
    parent->children.push_back(read_node);
}

void AST::build_print(ASTNode *parent, Node *stmt_node)
{
    /* print node */
    ASTNode *print_node = new ASTNode;
    print_node->type = ASTNode::PRINT;

    /* set print node to be children of parent */
    parent->children.push_back(print_node);

    /* add expr node tree for print node */
    build_expr(print_node, stmt_node->children[1]);
}

void AST::build_assert(ASTNode *parent, Node *stmt_node)
{
    /* assert node */
    ASTNode *assert_node = new ASTNode;
    assert_node->type = ASTNode::ASSERT;

    /* set assert node to be children of parent */
    parent->children.push_back(assert_node);

    /* add expr node tree for assert node */
    build_expr(assert_node, stmt_node->children[2]);
}

void AST::build_expr(ASTNode *parent, Node *expr_node)
{
    if (expr_node->children[0]->token.type == Token::OP_NOT) {
        /* unary operator */
        ASTNode *unary_node = new ASTNode;
        unary_node->type = ASTNode::UNARY_OP;
        parent->children.push_back(unary_node);

        build_opnd(unary_node, expr_node->children[1]);
    } else {
        ASTNode *op_node = new ASTNode;
        op_node->type = ASTNode::OPERATOR;
        
        switch (expr_node->children[1]->token.type) {
            case Token::OP_ADD:
                op_node->operator_type = ASTOperator::ADD;
                break;
            case Token::OP_SUBT:
                op_node->operator_type = ASTOperator::SUBTRACT;
                break;
            case Token::OP_DIVIS:
                op_node->operator_type = ASTOperator::DIVIDE;
                break;
            case Token::OP_NOT:
                op_node->operator_type = ASTOperator::NOT;
                break;
            case Token::OP_MULT:
                op_node->operator_type = ASTOperator::MULTIPLY;
                break;
            case Token::OP_AND:
                op_node->operator_type = ASTOperator::AND;
                break;
            case Token::OP_LT:
                op_node->operator_type = ASTOperator::LESS_THAN;
                break;
            case Token::OP_EQ:
                op_node->operator_type = ASTOperator::EQUALS;
                break;
            default:
                printf("Error: Operator type cannot be defined.\n");
        }

        parent->children.push_back(op_node);

        build_opnd(op_node, expr_node->children[0]);
        build_opnd(op_node, expr_node->children[2]);
    }
}

void AST::build_opnd(ASTNode *parent, Node *opnd_node)
{
    ASTNode *wat_node = NULL;
    switch (opnd_node->children[0]->token.type) {
        case Token::INTEGER:
            wat_node = new ASTNode;
            wat_node->type = ASTNode::CONSTANT;
            wat_node->value = opnd_node->children[0]->token.str;
            wat_node->variable_type = ASTVariable::INTEGER;
            parent->children.push_back(wat_node);
            break;
        case Token::STRING:
            wat_node = new ASTNode;
            wat_node->type = ASTNode::CONSTANT;
            wat_node->value = opnd_node->children[0]->token.str;
            wat_node->variable_type = ASTVariable::STRING;
            parent->children.push_back(wat_node);
            break;
        case Token::IDENTIFIER:
            wat_node = new ASTNode;
            wat_node->type = ASTNode::VAR_ID;
            wat_node->value = opnd_node->children[0]->token.str;
            wat_node->variable_type = ASTVariable::UNKNOWN;
            parent->children.push_back(wat_node);
            break;
        case Token::BRACKET_LEFT:
            build_expr(parent, opnd_node->children[1]);
            break;
        default:
            printf("Error: Parse tree incorrect.\n");
    }
}

} // namespace mpli
