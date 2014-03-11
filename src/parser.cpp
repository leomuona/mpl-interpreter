#include "parser.hpp"

namespace mpli {

void Parser::match(Token::TYPE expected)
{
	if (_curr_token.type == expected) {
		get_next_token();
	} else {
		token_error();
	}
}

void Parser::get_next_token()
{
	_curr_token = _scanner->next_token();
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
	get_next_token();
	// TODO
}

} // namespace mpli
