#ifndef MPLI_PARSER_HPP_
#define MPLI_PARSER_HPP_

#include "token.hpp"
#include "node.hpp"
#include "scanner.hpp"
#include "ast.hpp"

namespace mpli {

/* 
 * Parser to parse token stream into parse tree and later creating AST.
 */
class Parser {
    private:
		Scanner *_scanner;

        Node *_root_node;

		Token _curr_token;

        int _n_errors;

		/* utility functions */
        Node *new_node(Node::TYPE type);
        Node *new_token_node(Token token);
        int match(Token::TYPE expected);
		void parse_child_node(Token::TYPE expected, Node *parent);
        void token_error();
        void delete_node_r(Node *node);
		void debug_print_r(Node *node, int level);

		/* black magic and ugly code */
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
		/* set scanner that is used */
		void set_scanner(Scanner *scanner);
		/* start the token stream parsing into parse tree*/
		void start();
		/* returns number of errors reported */
        int number_of_errors();
		/* create AST into given AST pointer */
		void create_ast(AST *ast);
		/* prints debug parse tree with level information */
		void debug_print();
};

} // namespace mpli
#endif // MPLI_PARSER_HPP_
