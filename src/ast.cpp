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
    
    /* add expr node tree for insert node */
    build_expr(insert_node, stmt_node->children[2]);
}

void AST::build_for_loop(ASTNode *parent, Node *stmt_node)
{

}

void AST::build_read(ASTNode *parent, Node *stmt_node)
{

}

void AST::build_print(ASTNode *parent, Node *stmt_node)
{

}

void AST::build_assert(ASTNode *parent, Node *stmt_node)
{

}

void AST::build_expr(ASTNode *parent, Node *expr_node)
{

}

} // namespace mpli
