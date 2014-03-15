#include "interpreter.hpp"

#include <cstdio>

namespace mpli {

int Interpreter::execute(AST *ast)
{
	printf("INFO: Interpreter::execute - Executing program.\n");
	ASTNode *root = ast->root();
	if (!root) {
		printf("ERROR: Interpreter::execute - AST root is not valid.\n");
	}
	int r = 0;
	for (int i=0; i < root->children.size(); ++i) {
		if (r != 0) {
			/* error -> exit with error code */
			return r;
		}
		switch (root->children[i]->type) {
			case ASTNode::INSERT:
				r = execute_insert(root->children[i]);
				break;
			case ASTNode::FOR_LOOP:
				r = execute_for_loop(root->children[i]);
				break;
			case ASTNode::VAR_INIT:
				r = execute_var_init(root->children[i]);
				break;
			case ASTNode::READ:
				r = execute_read(root->children[i]);
				break;
			case ASTNode::PRINT:
				r = execute_print(root->children[i]);
				break;
			case ASTNode::ASSERT:
				r = execute_assert(root->children[i]);
				break;
			default:
				printf("ERROR: Interpreter::execute - AST root's child is not valid.\n");
				r = 1;
		}
	}
	return 0;
}

int Interpreter::execute_var_init(ASTNode *node)
{
	switch (node->children[0]->variable_type) {
		case ASTVariable::INTEGER:
			
			break;
		case ASTVariable::STRING:
		
			break;
		case ASTVariable::BOOLEAN:

			break;
		default:
			printf("ERROR: Interpreter::execute_var_init - Variable type is not valid.\n");
			return 1;
	}
	return 0;
}

int Interpreter::execute_insert(ASTNode *node)
{

	return 0;
}

int Interpreter::execute_for_loop(ASTNode *node)
{

	return 0;
}

int Interpreter::execute_read(ASTNode *node)
{

	return 0;
}

int Interpreter::execute_print(ASTNode *node)
{

	return 0;
}

int Interpreter::execute_assert(ASTNode *node)
{

	return 0;
}

} // namespace mpli
