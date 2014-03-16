#include "interpreter.hpp"

#include <cstdio>
#include <iostream>
#include <stdexcept>

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
	if (_symbol_table.find(node->children[0]->value).type == Symbol::UNDEFINED) {
		printf("ERROR: Interpreter::execute_var_init - Identifier %s is already initialized.\n", node->children[0]->value.c_str());
		return 1;
	}

	Symbol s;
	switch (node->children[0]->variable_type) {
		case ASTVariable::INTEGER:
			s.type = Symbol::VARIABLE_INT;
			s.location = _int_values.size();
			_int_values.push_back(0);
			_symbol_table.push(node->children[0]->value, s);
			break;
		case ASTVariable::STRING:
			s.type = Symbol::VARIABLE_STRING;
			s.location = _string_values.size();
			_string_values.push_back("");
			_symbol_table.push(node->children[0]->value, s);	
			break;
		case ASTVariable::BOOLEAN:
			s.type = Symbol::VARIABLE_BOOL;
			s.location = _bool_values.size();
			_bool_values.push_back(0);
			_symbol_table.push(node->children[0]->value, s);
			break;
		default:
			printf("ERROR: Interpreter::execute_var_init - Variable type is not valid.\n");
			return 1;
	}
	return 0;
}

int Interpreter::execute_insert(ASTNode *node)
{
	/* children[0] == id_node */
	Symbol s = _symbol_table.find(node->children[0]->value);
	if (s.type == Symbol::UNDEFINED) {
		printf("ERROR: Interpreter::execute_insert - Identifier %s is not initialized.\n", node->children[0]->value.c_str());
		return 1;
	}

	Symbol s2;
	switch (node->children[1]->type) {
		case ASTNode::OPERATOR:
			switch (s.type) {
				case Symbol::VARIABLE_INT:
					_int_values[s.location] = int_calc_op(node->children[1]);
					break;
				case Symbol::VARIABLE_STRING:
					_string_values[s.location] = string_calc_op(node->children[1]);
					break;
				case Symbol::VARIABLE_BOOL:
					_bool_values[s.location] = bool_calc_op(node->children[1]);
					break;
				default:
					printf("ERROR: Interpreter::execute_insert - Identifier typing error.\n");
					return 1;
			}
			break;
		case ASTNode::UNARY_OP:
			if (s.type != Symbol::VARIABLE_BOOL) {
				printf("ERROR: Interpreter::execute_insert - Unary operator '!' for non-boolean variable is not allowed.\n");
				return 1;
			}
			_bool_values[s.location] = calc_unary_op(node->children[1]);
			break;
		case ASTNode::VAR_ID:
			s2 = _symbol_table.find(node->children[1]->value);
			if (s.type != s2.type) {
				printf("ERROR: Interpreter::execute_insert - Identifier type miss match for identifiers %s and %s.\n",
					node->children[0]->value.c_str(), node->children[1]->value.c_str());
				return 1;
			}
			switch (s.type) {
				case Symbol::VARIABLE_INT:
					_int_values[s.location] = _int_values[s2.location];
					break;
				case Symbol::VARIABLE_STRING:
					_string_values[s.location] = _string_values[s2.location];
					break;
				case Symbol::VARIABLE_BOOL:
					_bool_values[s.location] = _bool_values[s2.location];
					break;
				default:
					printf("ERROR: Interpreter::execute_insert - Identifier typing error.\n");
					return 1;
			}
			break;
		case ASTNode::CONSTANT:
			switch (s.type) {
				case Symbol::VARIABLE_INT:
					_int_values[s.location] = to_int(node->children[1]->value);
					break;
				case Symbol::VARIABLE_STRING:
					_string_values[s.location] = node->children[1]->value;
					break;
				default:
					printf("ERROR: Interpreter::execute_insert - Cannot insert constant into bool value.\n");
					return 1;
			}
			break;
		default:
			printf("ERROR: Interpereter::execute_var_init - Insert statement is not valid.\n");
			return 1;
	}

	return 0;
}

int Interpreter::execute_for_loop(ASTNode *node)
{
	int start = 0, end = 0;

	/* in_node */
	ASTNode *in_node = node->children[0];
	Symbol s = _symbol_table.find(in_node->children[0]->value);
	if (s.type != Symbol::VARIABLE_INT) {
		printf("ERROR: Interpreter::execute_for_loop - Identifier %s not found or wrong typing.\n",
			in_node->children[0]->value.c_str());
		return 1;
	}
	/* start */
	Symbol s2;
	switch (in_node->children[1]->type) {
		case ASTNode::OPERATOR:
			start = int_calc_op(in_node->children[1]);			
			break;
		case ASTNode::VAR_ID:
			s2 = _symbol_table.find(in_node->children[1]->value);
			if (s2.type != Symbol::VARIABLE_INT) {
				printf("ERROR: Interpreter::execute_for_loop - Identifier %s not found or wrong typing.\n",
					in_node->children[1]->value.c_str());
			}
			start = _int_values[s2.location];
			break;
		case ASTNode::CONSTANT:
			start = to_int(in_node->children[1]->value);
			break;
		default:
			printf("ERROR: Interpreter::execute_for_loop - Invalid range type for FOR_LOOP.\n");
			return 1;
	}
	/* end */
	switch (in_node->children[2]->type) {
		case ASTNode::OPERATOR:
			end = int_calc_op(in_node->children[2]);			
			break;
		case ASTNode::VAR_ID:
			s2 = _symbol_table.find(in_node->children[2]->value);
			if (s2.type != Symbol::VARIABLE_INT) {
				printf("ERROR: Interpreter::execute_for_loop - Identifier %s not found or wrong typing.\n",
					in_node->children[2]->value.c_str());
			}
			end = _int_values[s2.location];
			break;
		case ASTNode::CONSTANT:
			end = to_int(in_node->children[2]->value);
			break;
		default:
			printf("ERROR: Interpreter::execute_for_loop - Invalid range type for FOR_LOOP.\n");
			return 1;
	}

	if (end < start) {
		printf("ERROR: Interpreter::execute_for_loop - Invalid range defined %d..%d\n", start, end);
		return 1;
	}

	/* DO-PART */
	ASTNode *do_node = node->children[1];
	int r = 0;

	/* EXECUTE */
	for(int i=start; i <= end; ++i) {
		/* set identifier value */
		_int_values[s.location] = i;

		for (int i=0; i < do_node->children.size(); ++i) {
			if (r != 0) {
				/* error -> exit with error code */
				return r;
			}
			switch (do_node->children[i]->type) {
				case ASTNode::INSERT:
					r = execute_insert(do_node->children[i]);
					break;
				case ASTNode::FOR_LOOP:
					r = execute_for_loop(do_node->children[i]);
					break;
				case ASTNode::VAR_INIT:
					r = execute_var_init(do_node->children[i]);
					break;
				case ASTNode::READ:
					r = execute_read(do_node->children[i]);
					break;
				case ASTNode::PRINT:
					r = execute_print(do_node->children[i]);
					break;
				case ASTNode::ASSERT:
					r = execute_assert(do_node->children[i]);
					break;
				default:
					printf("ERROR: Interpreter::execute_for_loop - Invalid statement.\n");
					r = 1;
			}
		}
	}

	return 0;
}

int Interpreter::execute_read(ASTNode *node)
{
	if (node->children[0]->type != ASTNode::VAR_ID) {
		printf("ERROR: Interpreter::execute_read - Invalid read statement.\n");
		return 1;
	}
	Symbol s = _symbol_table.find(node->children[0]->value);
	
	int i;
	std::string str;
	switch (s.type) {
		case Symbol::VARIABLE_INT:
			std::cin >> i;
			_int_values[s.location] = i;
			break;
		case Symbol::VARIABLE_STRING:
			std::cin >> str;
			_string_values[s.location] = str;
			break;
		case Symbol::VARIABLE_BOOL:
			printf("ERROR: Interpreter::execute_read - Boolean type identifier cannot be used in read statement.\n");
			return 1;
			break;
		default:
			printf("ERROR: Interpreter::execute_read - Identifier %s not initialized.\n", node->children[0]->value.c_str());
			return 1;
	}

	return 0;
}

int Interpreter::execute_print(ASTNode *node)
{
	Symbol s;
	ASTVariable::TYPE t = ASTVariable::UNKNOWN;
	switch (node->children[0]->type) {
		case ASTNode::UNARY_OP:
			if (calc_unary_op(node->children[0])) {
				printf("true");
			} else {
				printf("false");
			}
			break;
		case ASTNode::OPERATOR:
			t = op_var_typing(node->children[0]);
			switch (t) {
				case ASTVariable::INTEGER:
					printf("%d", int_calc_op(node->children[0]));
					break;
				case ASTVariable::STRING:
					printf("%s", string_calc_op(node->children[0]).c_str());
					break;
				case ASTVariable::BOOLEAN:
					if (bool_calc_op(node->children[0])) {
						printf("true");
					} else {
						printf("false");
					}
					break;
				default:
					printf("ERROR: Interpreter::execute_print - Could not define typing for operator.\n");
					return 1;
			}

			break;
		case ASTNode::VAR_ID:
			s = _symbol_table.find(node->children[0]->value);
			switch (s.type) {
				case Symbol::VARIABLE_INT:
					printf("%d", _int_values[s.location]);
					break;
				case Symbol::VARIABLE_STRING:
					printf("%s", _string_values[s.location].c_str());
					break;
				case Symbol::VARIABLE_BOOL:
					if (_bool_values[s.location]) {
						printf("true");
					} else {
						printf("false");
					}
					break;
				default:
					printf("ERROR: Interpreter::execute_print - Identifier %s is not initialized.\n",
						node->children[0]->value.c_str());
					return 1;
			}
			break;
		case ASTNode::CONSTANT:
			printf("%s", node->children[0]->value.c_str());
			break;
		default:
			printf("ERROR: Interpreter::execute_print - Invalid print statement.\n");
			return 1;
	}

	return 0;
}

int Interpreter::execute_assert(ASTNode *node)
{
	int fail = 0;
	Symbol s;
	switch (node->children[0]->type) {
		case ASTNode::UNARY_OP:
			if (! calc_unary_op(node->children[0])) {
				fail = 1;
			}
			break;
		case ASTNode::OPERATOR:
			if (! bool_calc_op(node->children[0])) {
				fail = 1;
			}
			break;
		case ASTNode::VAR_ID:
			s = _symbol_table.find(node->children[0]->value);
			if (s.type != Symbol::VARIABLE_BOOL) {
				printf("ERROR: Interpreter::execute_assert - %s is non-bool identifier or identifier not initialized.\n",
					node->children[0]->value.c_str());
				return 1;
			}
			if (! _bool_values[s.location]) {
				fail = 1;
			}
			break;
		default:
			printf("ERROR: Interpreter::execute_assert - Assert statement is not valid.\n");
			return 1;
	}

	if (fail) {
		printf("ERROR: Interpreter::execute_assert - Assert returned false. Cannot continue.\n");
		return 1;
	}

	return 0;
}

int Interpreter::int_calc_op(ASTNode *node)
{
	int result = 0;
	int left = int_for_op(node->children[0]);
	int right = int_for_op(node->children[1]);
	/* calculate */
	switch (node->operator_type) {
		case ASTOperator::ADD:
			result = left + right;
			break;
		case ASTOperator::SUBTRACT:
			result = left - right;
			break;
		case ASTOperator::MULTIPLY:
			result = left * right;
			break;
		case ASTOperator::DIVIDE:
			if (right == 0) {
				throw std::invalid_argument("Cannot divide by zero.");
			}
			result = left / right;
			break;
		default:
			throw std::invalid_argument("Non-valid operator for int return value.");
	}
	return result;
}

std::string Interpreter::string_calc_op(ASTNode *node)
{
	std::string result = "";
	std::string left = string_for_op(node->children[0]);
	std::string right = string_for_op(node->children[1]);
	/* calculate */
	switch (node->operator_type) {
		case ASTOperator::ADD:
			result.append(left);
			result.append(right);
			break;
		default:
			throw std::invalid_argument("Non-valid operator for int return value.");
	}
	return result;
}

int Interpreter::bool_calc_op(ASTNode *node)
{
	int result = 0;
	
	int left = 0, right = 0;
	ASTVariable::TYPE t = op_var_typing(node);
	/* calculate */
	switch (node->operator_type) {
		case ASTOperator::LESS_THAN:
			/* both are integers */
			result = (int_for_op(node->children[0]) < int_for_op(node->children[1]));
			break;
		case ASTOperator::EQUALS:
			/* calculate equals */
			switch (t) {
				case ASTVariable::INTEGER:
					result = (int_for_op(node->children[0]) == int_for_op(node->children[1]));
					break;
				case ASTVariable::STRING:
					result = (string_for_op(node->children[0]) == string_for_op(node->children[1]));
					break;
				case ASTVariable::BOOLEAN:
					left = bool_for_op(node->children[0]);
					right = bool_for_op(node->children[1]);
					if ((left && right) || !(left || right)) {
						result = 1;
					} else {
						result = 0;
					}
					break;
				default:
					throw std::invalid_argument("Non-valid type for operator EQUALS.");
			}
			break;
		case ASTOperator::AND:
			/* both are booleans */
			result = (bool_for_op(node->children[0]) && bool_for_op(node->children[1]));	
			break;
		case ASTOperator::NOT:
			/* calculate not */
			switch (t) {
				case ASTVariable::INTEGER:
					result = (int_for_op(node->children[0]) != int_for_op(node->children[1]));
					break;
				case ASTVariable::STRING:
					result = (string_for_op(node->children[0]) != string_for_op(node->children[1]));
					break;
				case ASTVariable::BOOLEAN:
					left = bool_for_op(node->children[0]);
					right = bool_for_op(node->children[1]);
					if ((left && !right) || (!left && right)) {
						result = 1;
					} else {
						result = 0;
					}
					break;
				default:
					throw std::invalid_argument("Non-valid type for operator NOT.");
			}
			break;
		default:
			throw std::invalid_argument("Non-valid operator for bool return value.");
	}

	return result;
}

int Interpreter::calc_unary_op(ASTNode *node)
{	
	return (!bool_for_op(node->children[0]));
}

int Interpreter::int_for_op(ASTNode *node)
{
	int result = 0;

	Symbol s;
	std::string e_str;
	switch (node->type) {
		case ASTNode::OPERATOR:
			result = int_calc_op(node);
			break;
		case ASTNode::VAR_ID:
			s = _symbol_table.find(node->value);
			if (s.type != Symbol::VARIABLE_INT) {
				e_str = "Identifier ";
				e_str.append(node->value);
				e_str.append(" not found or has wrong typing.");
				throw std::invalid_argument(e_str.c_str());
			}
			result = _int_values[s.location];
			break;
		case ASTNode::CONSTANT:
			result = to_int(node->value);
			break;
		default:
			throw std::invalid_argument("Invalid argument for operator.");
	}

	return result;
}

std::string Interpreter::string_for_op(ASTNode *node)
{
	std::string result = "";

	Symbol s;
	std::string e_str;
	switch (node->type) {
		case ASTNode::OPERATOR:
			result = string_calc_op(node);
			break;
		case ASTNode::VAR_ID:
			s = _symbol_table.find(node->value);
			switch (s.type) {
				case Symbol::VARIABLE_STRING:
					result = _string_values[s.location];
					break;
				case Symbol::VARIABLE_INT:
					result = to_string(_int_values[s.location]);
					break;
				case Symbol::VARIABLE_BOOL:
					if (_bool_values[s.location]) {
						result = "true";
					} else {
						result = "false";
					}
				default:
					e_str = "Identifier ";
					e_str.append(node->value);
					e_str.append(" not found.");
					throw std::invalid_argument(e_str.c_str());
			}
			break;
		case ASTNode::CONSTANT:
			result = node->value;
			break;
		default:
			throw std::invalid_argument("Invalid argument for operator.");
	}

	return result;
}

int Interpreter::bool_for_op(ASTNode *node)
{
	int result = 0;

	Symbol s;
	std::string e_str;
	switch (node->type) {
		case ASTNode::UNARY_OP:
			result = calc_unary_op(node);
			break;
		case ASTNode::OPERATOR:
			result = bool_calc_op(node);
			break;
		case ASTNode::VAR_ID:
			s = _symbol_table.find(node->value);
			if (s.type != Symbol::VARIABLE_BOOL) {
				e_str = "Identifier ";
				e_str.append(node->value);
				e_str.append(" not found or has wrong typing.");
				throw std::invalid_argument(e_str.c_str());
			}
			result = _bool_values[s.location];
			break;
		default:
			throw std::invalid_argument("Invalid argument for operator.");

	}

	return result;
}

ASTVariable::TYPE Interpreter::op_var_typing(ASTNode *node)
{
	ASTVariable::TYPE t = ASTVariable::UNKNOWN;
	ASTNode *n = node;
	switch (node->children[0]->type) {
		case ASTNode::OPERATOR:
			n = node->children[0]->children[0];
			while (n->type == ASTNode::OPERATOR) {
				n = n->children[0];
			}
			switch (n->type) {
				case ASTNode::UNARY_OP:
					t = ASTVariable::BOOLEAN;
					break;
				case ASTNode::VAR_ID:
				case ASTNode::CONSTANT:
					t = n->variable_type;
					break;
				default:
				t = ASTVariable::UNKNOWN;
			}
			break;
		case ASTNode::UNARY_OP:
			t = ASTVariable::BOOLEAN;
			break;
		case ASTNode::VAR_ID:
		case ASTNode::CONSTANT:
			t = node->children[0]->variable_type;
			break;
		default:
			t = ASTVariable::UNKNOWN;
	}
	return t;
}

int Interpreter::to_int(std::string str)
{
	const char *s = str.c_str();
	if (*s == '\0') {
		throw std::invalid_argument("Empty string argument.");
	}

	int neg = (s[0] == '-');
	if (s[0] == '+' || s[0] == '-') {
		++s;
	}
	if (*s == '\0') {
		throw std::invalid_argument("String argument has only sign character.");
	}

	int r = 0;
	while (*s) {
		if (*s >= '0' && *s <= '9') {
			r = r * 10  - (*s - '0');  //assume neg number
		} else {
			throw std::invalid_argument("Invalid string argument.");
		}
		++s;
	}

	return neg ? r : -r;
}

std::string Interpreter::to_string(int val)
{
	char numstr[21]; // enough to hold all numbers up to 64-bits
	sprintf(numstr, "%d", val);
	return std::string(numstr);
}

} // namespace mpli
