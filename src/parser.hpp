#ifndef MPLI_PARSER_HPP_
#define MPLI_PARSER_HPP_

#include "token.hpp"
#include "node.hpp"
#include "scanner.hpp"
#include "ast.hpp"

namespace mpli {

class Parser {
    private:
		Scanner *_scanner;

        Node *_root_node;

		Token _curr_token;

        int _n_errors;

        Node *new_node(Node::TYPE type);
        Node *new_token_node(Token token);
        int match(Token::TYPE expected);
		void parse_child_node(Token::TYPE expected, Node *parent);
        void token_error();
        void delete_node_r(Node *node);
		void debug_print_r(Node *node, int level);

		void parse_prog();
		void parse_stmts(Node *parent);
		void parse_stmt(Node *parent);
		void parse_expr(Node *parent);
		void parse_opnd(Node *parent);
		void parse_type(Node *parent);
		void parse_op(Node *parent);

    public:
        Parser();
        ~Parser();
		void set_scanner(Scanner *scanner);
		void start();
        int number_of_errors();
		void create_ast(AST *ast);
		void debug_print();
};

} // namespace mpli
#endif // MPLI_PARSER_HPP_
