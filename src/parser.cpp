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
				  _curr_token.type == Token::KW_IDENTIFIER ||
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
	// TODO: stuff
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

			break;
		/* "for" <var_ident> "in" <expr> ".." <expr> "do" <stmts> "end" "for" */
		case Token::KW_FOR:

			break;
		/* "read" <var_ident> */
		case Token::KW_READ:

			break;
		/* "print" <var_ident> */
		case Token::KW_PRINT:

			break;
		/* "assert" "(" <expr> ")" */
		case Token::KW_ASSERT:

			break;
		/* I AM ERROR */
		default:
			token_error();
	}
}

void Parser::parse_expr()
{
	// TODO
}

void Parser::parse_opnd()
{
	// TODO
}

void Parser::parse_type()
{
	// TODO
}

} // namespace mpli
