#include "parser.hpp"

namespace mpli {

void Parser::match(Token::TYPE expected)
{
	if (_curr_token.type == expected) {
		_curr_token = _scanner->next_token();
	} else {
		token_error();
	}
}

void Parser::token_error()
{
	// TODO: print error
}

void Parser::set_scanner(Scanner *scanner)
{
	_scanner = scanner;
}

void Parser::start()
{
	_curr_token = _scanner->next_token();
	// TODO: stuff
	parse_prog();
}

void Parser::parse_prog()
{
	parse_stmts();
	match(Token::END_OF_FILE);
}

void Parser::parse_stmts()
{
	// TODO: stuff?
	switch (_curr_token.type) {
		/* <stmt> ";" ( <stmt> ";" )* */
		case Token::KW_VAR:
		case Token::IDENTIFIER:
		case Token::KW_FOR:
		case Token::KW_READ:
		case Token::KW_PRINT:
		case Token::KW_ASSERT:
			parse_stmt();
			match(Token::SEMICOLON);
			while(_curr_token.type == Token::KW_VAR ||
				  _curr_token.type == Token::IDENTIFIER ||
				  _curr_token.type == Token::KW_FOR ||
				  _curr_token.type == Token::KW_READ ||
				  _curr_token.type == Token::KW_PRINT ||
				  _curr_token.type == Token::KW_ASSERT) {
				parse_stmt();
				match(Token::SEMICOLON);
			}
			break;
		default:
			token_error();
	}
}

void Parser::parse_stmt()
{
	// TODO: stuff?
	switch (_curr_token.type) {
		/* "var" <var_ident> ":" <type> [ ":=" <expr> ] */
		case Token::KW_VAR:
			match(Token::KW_VAR);
			match(Token::IDENTIFIER);
			match(Token::COLON);
			parse_type();
			if (_curr_token.type == Token::INSERT) {
				match(Token::INSERT);
				parse_expr();
			}
			break;
		/* <var_ident> ":=" <expr> */
		case Token::IDENTIFIER:
			match(Token::IDENTIFIER);
			match(Token::INSERT);
			parse_expr();
			break;
		/* "for" <var_ident> "in" <expr> ".." <expr> "do" <stmts> "end" "for" */
		case Token::KW_FOR:
			match(Token::KW_FOR);
			match(Token::IDENTIFIER);
			match(Token::KW_IN);
			parse_expr();
			match(Token::DOUBLEDOT);
			parse_expr();
			match(Token::KW_DO);
			parse_stmts();
			match(Token::KW_END);
			match(Token::KW_FOR);
			break;
		/* "read" <var_ident> */
		case Token::KW_READ:
			match(Token::KW_READ);
			match(Token::IDENTIFIER);
			break;
		/* "print" <var_ident> */
		case Token::KW_PRINT:
			match(Token::KW_PRINT);
			match(Token::IDENTIFIER);
			break;
		/* "assert" "(" <expr> ")" */
		case Token::KW_ASSERT:
			match(Token::KW_ASSERT);
			match(Token::BRACKET_LEFT);
			parse_expr();
			match(Token::BRACKET_RIGHT);
			break;
		/* I AM ERROR */
		default:
			token_error();
	}
}

void Parser::parse_expr()
{
	// TODO: stuff?
	switch (_curr_token.type) {
		/* <opnd> <op> <opnd> | <opnd> */
		case Token::INTEGER:
		case Token::STRING:
		case Token::IDENTIFIER:
		case Token::BRACKET_LEFT:
			parse_opnd();
			if (_curr_token.type == Token::OP_ADD || 
			    _curr_token.type == Token::OP_SUBS ||
				_curr_token.type == Token::OP_DIVIS ||
				_curr_token.type == Token::OP_NOT ||
				_curr_token.type == Token::OP_MULT ||
				_curr_token.type == Token::OP_AND ||
				_curr_token.type == Token::OP_LT ||
				_curr_token.type == Token::OP_EQ) {
				parse_op();
				parse_opnd();
			}
			break;
		/* [ <unary_op> ] <opnd> */
		case Token::OP_NOT:
			match(Token::OP_NOT);
			parse_opnd();
			break;
		default:
			token_error();
	}
}

void Parser::parse_opnd()
{
	// TODO: stuff?
	switch (_curr_token.type) {
		/* <int> */
		case Token::INTEGER:
			match(Token::INTEGER);
			break;
		/* <string> */
		case Token::STRING:
			match(Token::STRING);
			break;
		/* <var_ident> */
		case Token::IDENTIFIER:
			match(Token::IDENTIFIER);
			break;
		/* "(" expr ")" */
		case Token::BRACKET_LEFT:
			match(Token::BRACKET_LEFT);
			parse_expr();
			match(Token::BRACKET_RIGHT);
			break;
		default:
			token_error();
	}
}

void Parser::parse_type()
{
	// TODO: stuff?
	switch (_curr_token.type) {
		case Token::KW_INT:
			match(Token::KW_INT);
			break;
		case Token::KW_STRING:
			match(Token::KW_STRING);
			break;
		case Token::KW_BOOL:
			match(Token::KW_BOOL);
			break;
		default:
			token_error();
	}
}

void Parser::parse_op()
{
	// TODO: stuff?
	switch (_curr_token.type) {
		case Token::OP_ADD:
			match(Token::OP_ADD);
			break;
		case Token::OP_SUBS:
			match(Token::OP_SUBS);
			break;
		case Token::OP_DIVIS:
			match(Token::OP_DIVIS);
			break;
		case Token::OP_NOT:
			match(Token::OP_NOT);
			break;
		case Token::OP_MULT:
			match(Token::OP_MULT);
			break;
		case Token::OP_AND:
			match(Token::OP_AND);
			break;
		case Token::OP_LT:
			match(Token::OP_LT);
			break;
		case Token::OP_EQ:
			match(Token::OP_EQ);
			break;
		default:
			token_error();
	}

}

} // namespace mpli
