#ifndef MPLI_TOKEN_HPP_
#define MPLI_TOKEN_HPP_

#include <string>

namespace mpli {

/*
 * Define Token struct that is used in token stream and parse tree.
 */
struct Token {
    
    enum TYPE { IDENTIFIER, INTEGER, STRING, DOUBLEDOT, COLON, INSERT, SEMICOLON,
                BRACKET_RIGHT, BRACKET_LEFT, OP_ADD, OP_SUBT, OP_DIVIS, OP_NOT,
                OP_MULT, OP_AND, OP_LT, OP_EQ, KW_VAR, KW_FOR, KW_END, KW_IN, KW_DO, KW_READ,
                KW_PRINT, KW_INT, KW_STRING, KW_BOOL, KW_ASSERT, END_OF_FILE, ERROR };

    TYPE type;
    std::string str;

    Token(TYPE t, std::string s) : type(t), str(s) { }
    Token() { type = ERROR; str = ""; }

	std::string type_str()
	{
		const char* types[] = { "IDENTIFIER", "INTEGER", "STRING", "DOUBLEDOT", "COLON", "INSERT", "SEMICOLON",
                "BRACKET_RIGHT", "BRACKET_LEFT", "OP_ADD", "OP_SUBT", "OP_DIVIS", "OP_NOT",
                "OP_MULT", "OP_AND", "OP_LT", "OP_EQ", "KW_VAR", "KW_FOR", "KW_END", "KW_IN", "KW_DO", "KW_READ",
                "KW_PRINT", "KW_INT", "KW_STRING", "KW_BOOL", "KW_ASSERT", "END_OF_FILE", "ERROR" };
		return std::string(types[type]);
	}
};

} // namespace mpli
#endif // MPLI_TOKEN_HPP_
