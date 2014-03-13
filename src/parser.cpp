#include "parser.hpp"
#include <cstdio>

namespace mpli {

Parser::~Parser()
{
    /* delete parse tree recursively */
    delete_node_r(_root_node);
}

Node *Parser::new_node(Node::TYPE type)
{
    Node *node = new Node;
    node->type = type;
    return node;
}

Node *Parser::new_token_node(Token token)
{
    Node *node = new Node;
    node->type = Node::TOKEN;
    node->token = token;
    return node;
}

int Parser::match(Token::TYPE expected)
{
	if (_curr_token.type == expected) {
        printf("Valid token: %d %s\n", _curr_token.type, _curr_token.str.c_str());
		_curr_token = _scanner->next_token();
	    return 1;
    } else {
		token_error();
        return 0;
	}
}

void Parser::parse_child_node(Token::TYPE expected, Node *parent)
{
    Token t = _curr_token;
    if (match(expected)) {
        Node *node = new_token_node(t);
        parent->children.push_back(node);
    }
}

void Parser::token_error()
{
    if (_curr_token.type == Token::ERROR) {
        printf("Error: cannot resolve token type for token '%s'\n",
               _curr_token.str.c_str());
    } else {
        printf("Error: '%s' is not a valid token for this location.\n", 
               _curr_token.str.c_str());
    }
}

void Parser::delete_node_r(Node *node)
{
    std::vector<Node*>::iterator it;
    for(it = node->children.begin(); it != node->children.end(); it++) {
        delete_node_r((*it));
    }
    delete node;
}

void Parser::set_scanner(Scanner *scanner)
{
	_scanner = scanner;
}

void Parser::start()
{
	_curr_token = _scanner->next_token();
	parse_prog();
}

void Parser::parse_prog()
{
    _root_node = new_node(Node::PROG);
	parse_stmts(_root_node);
	match(Token::END_OF_FILE);
}

void Parser::parse_stmts(Node *parent)
{
    Node *stmts_node = new_node(Node::STMTS);
    parent->children.push_back(stmts_node);
	
    switch (_curr_token.type) {
		/* <stmt> ";" ( <stmt> ";" )* */
		case Token::KW_VAR:
		case Token::IDENTIFIER:
		case Token::KW_FOR:
		case Token::KW_READ:
		case Token::KW_PRINT:
		case Token::KW_ASSERT:
			parse_stmt(stmts_node);
			parse_child_node(Token::SEMICOLON, stmts_node);
			while(_curr_token.type == Token::KW_VAR ||
				  _curr_token.type == Token::IDENTIFIER ||
				  _curr_token.type == Token::KW_FOR ||
				  _curr_token.type == Token::KW_READ ||
				  _curr_token.type == Token::KW_PRINT ||
				  _curr_token.type == Token::KW_ASSERT) {
				parse_stmt(stmts_node);
				parse_child_node(Token::SEMICOLON, stmts_node);
			}
			break;
		default:
			token_error();
	}
}

void Parser::parse_stmt(Node *parent)
{
	Node *stmt_node = new_node(Node::STMT);
    parent->children.push_back(stmt_node);

    switch (_curr_token.type) {
		/* "var" <var_ident> ":" <type> [ ":=" <expr> ] */
		case Token::KW_VAR:
			parse_child_node(Token::KW_VAR, stmt_node);
			parse_child_node(Token::IDENTIFIER, stmt_node);
			parse_child_node(Token::COLON, stmt_node);
			parse_type(stmt_node);
			if (_curr_token.type == Token::INSERT) {
				parse_child_node(Token::INSERT, stmt_node);
				parse_expr(stmt_node);
			}
			break;
		/* <var_ident> ":=" <expr> */
		case Token::IDENTIFIER:
			parse_child_node(Token::IDENTIFIER, stmt_node);
			parse_child_node(Token::INSERT, stmt_node);
			parse_expr(stmt_node);
			break;
		/* "for" <var_ident> "in" <expr> ".." <expr> "do" <stmts> "end" "for" */
		case Token::KW_FOR:
			parse_child_node(Token::KW_FOR, stmt_node);
			parse_child_node(Token::IDENTIFIER, stmt_node);
			parse_child_node(Token::KW_IN, stmt_node);
			parse_expr(stmt_node);
			parse_child_node(Token::DOUBLEDOT, stmt_node);
			parse_expr(stmt_node);
			parse_child_node(Token::KW_DO, stmt_node);
			parse_stmts(stmt_node);
			parse_child_node(Token::KW_END, stmt_node);
			parse_child_node(Token::KW_FOR, stmt_node);
			break;
		/* "read" <var_ident> */
		case Token::KW_READ:
			parse_child_node(Token::KW_READ, stmt_node);
			parse_child_node(Token::IDENTIFIER, stmt_node);
			break;
		/* "print" <var_ident> */
		case Token::KW_PRINT:
			parse_child_node(Token::KW_PRINT, stmt_node);
			parse_expr(stmt_node);
			break;
		/* "assert" "(" <expr> ")" */
		case Token::KW_ASSERT:
			parse_child_node(Token::KW_ASSERT, stmt_node);
			parse_child_node(Token::BRACKET_LEFT, stmt_node);
			parse_expr(stmt_node);
			parse_child_node(Token::BRACKET_RIGHT, stmt_node);
			break;
		/* I AM ERROR */
		default:
			token_error();
	}
}

void Parser::parse_expr(Node *parent)
{
    Node *expr_node = new_node(Node::EXPR);
    parent->children.push_back(expr_node);

	switch (_curr_token.type) {
		/* <opnd> <op> <opnd> | <opnd> */
		case Token::INTEGER:
		case Token::STRING:
		case Token::IDENTIFIER:
		case Token::BRACKET_LEFT:
			parse_opnd(expr_node);
			if (_curr_token.type == Token::OP_ADD || 
			    _curr_token.type == Token::OP_SUBS ||
				_curr_token.type == Token::OP_DIVIS ||
				_curr_token.type == Token::OP_NOT ||
				_curr_token.type == Token::OP_MULT ||
				_curr_token.type == Token::OP_AND ||
				_curr_token.type == Token::OP_LT ||
				_curr_token.type == Token::OP_EQ) {
				parse_op(expr_node);
				parse_opnd(expr_node);
			}
			break;
		/* [ <unary_op> ] <opnd> */
		case Token::OP_NOT:
			parse_child_node(Token::OP_NOT, expr_node);
			parse_opnd(expr_node);
			break;
		default:
			token_error();
	}
}

void Parser::parse_opnd(Node *parent)
{
    Node *opnd_node = new_node(Node::OPND);
    parent->children.push_back(opnd_node);

	switch (_curr_token.type) {
		/* <int> */
		case Token::INTEGER:
			parse_child_node(Token::INTEGER, opnd_node);
			break;
		/* <string> */
		case Token::STRING:
			parse_child_node(Token::STRING, opnd_node);
			break;
		/* <var_ident> */
		case Token::IDENTIFIER:
			parse_child_node(Token::IDENTIFIER, opnd_node);
			break;
		/* "(" expr ")" */
		case Token::BRACKET_LEFT:
			parse_child_node(Token::BRACKET_LEFT, opnd_node);
			parse_expr(opnd_node);
			parse_child_node(Token::BRACKET_RIGHT, opnd_node);
			break;
		default:
			token_error();
	}
}

void Parser::parse_type(Node *parent)
{
	switch (_curr_token.type) {
		case Token::KW_INT:
			parse_child_node(Token::KW_INT, parent);
			break;
		case Token::KW_STRING:
			parse_child_node(Token::KW_STRING, parent);
			break;
		case Token::KW_BOOL:
			parse_child_node(Token::KW_BOOL, parent);
			break;
		default:
			token_error();
	}
}

void Parser::parse_op(Node *parent)
{
	switch (_curr_token.type) {
		case Token::OP_ADD:
			parse_child_node(Token::OP_ADD, parent);
			break;
		case Token::OP_SUBS:
			parse_child_node(Token::OP_SUBS, parent);
			break;
		case Token::OP_DIVIS:
			parse_child_node(Token::OP_DIVIS, parent);
			break;
		case Token::OP_NOT:
			parse_child_node(Token::OP_NOT, parent);
			break;
		case Token::OP_MULT:
			parse_child_node(Token::OP_MULT, parent);
			break;
		case Token::OP_AND:
			parse_child_node(Token::OP_AND, parent);
			break;
		case Token::OP_LT:
			parse_child_node(Token::OP_LT, parent);
			break;
		case Token::OP_EQ:
			parse_child_node(Token::OP_EQ, parent);
			break;
		default:
			token_error();
	}

}

} // namespace mpli
