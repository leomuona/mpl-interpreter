#ifndef MPLI_PARSER_HPP_
#define MPLI_PARSER_HPP_

#include "token.hpp"
#include "scanner.hpp"

namespace mpli {

class Parser {
    private:
		Scanner *_scanner;

		Token _curr_token;

		void match(Token::TYPE expected);
		void get_next_token();
		void token_error();

		void parse_prog();
		void parse_stmts();
		void parse_stmt();
		void parse_expr();
		void parse_opnd();
		void parse_type();

    public:
		void set_scanner(Scanner *scanner);
		void start();
};

} // namespace mpli
#endif // MPLI_PARSER_HPP_
