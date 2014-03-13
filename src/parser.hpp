#ifndef MPLI_PARSER_HPP_
#define MPLI_PARSER_HPP_

#include "token.hpp"
#include "node.hpp"
#include "scanner.hpp"

namespace mpli {

class Parser {
    private:
		Scanner *_scanner;

        Node *_root_node;

		Token _curr_token;

        Node *new_node(Node::TYPE type);
        Node *new_token_node(Token token);
        int match(Token::TYPE expected);
		void parse_child_node(Token::TYPE expected, Node *parent);
        void token_error();
        void delete_node_r(Node *node);

		void parse_prog();
		void parse_stmts(Node *parent);
		void parse_stmt(Node *parent);
		void parse_expr(Node *parent);
		void parse_opnd(Node *parent);
		void parse_type(Node *parent);
		void parse_op(Node *parent);

    public:
        ~Parser();
		void set_scanner(Scanner *scanner);
		void start();
};

} // namespace mpli
#endif // MPLI_PARSER_HPP_
