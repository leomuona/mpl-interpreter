#include "scanner.hpp"
#include <cstring>
#include <cstdio>

namespace mpli {

Scanner::Scanner()
{
    /* Constructing states table:
       Must be constructed in a priority order high-low.*/
    /* 0: initial state */
    std::vector<StateRow> v0;
    v0.push_back(StateRow(ALPHA, 10));
    v0.push_back(StateRow(DIGIT, 20));
    v0.push_back(StateRow(PERIOD, 30));
    v0.push_back(StateRow(COLON, 40));
    v0.push_back(StateRow(SEMICOLON, _TOKEN_END_STATE));
    v0.push_back(StateRow(BRACKET_RIGHT, _TOKEN_END_STATE));
    v0.push_back(StateRow(BRACKET_LEFT, _TOKEN_END_STATE));
    v0.push_back(StateRow(PLUS, _TOKEN_END_STATE));
    v0.push_back(StateRow(MINUS, _TOKEN_END_STATE));
    //v0.push_back(StateRow(SLASH, _TOKEN_END_STATE));
    v0.push_back(StateRow(SLASH, 60));
	v0.push_back(StateRow(EXCLAMATIONM, _TOKEN_END_STATE));
    v0.push_back(StateRow(ASTERISK, _TOKEN_END_STATE));
    v0.push_back(StateRow(AND, _TOKEN_END_STATE));
    v0.push_back(StateRow(QUOTEM, 50));
    v0.push_back(StateRow(EQUALS, _TOKEN_END_STATE));
    v0.push_back(StateRow(LESS_THAN, _TOKEN_END_STATE));
    _states_map[0] = v0;

    /* 10: aplha first: indentifier or keyword */
    std::vector<StateRow> v10;
    v10.push_back(StateRow(ALPHA, 10));
    v10.push_back(StateRow(DIGIT, 10));
    v10.push_back(StateRow(UNDERSCORE, 10));
    v10.push_back(StateRow(NOT_ANY, _TOKEN_BREAK_STATE, 1));
    _states_map[10] = v10;

    /* 20: digit first: int value */
    std::vector<StateRow> v20;
    v20.push_back(StateRow(DIGIT, 20));
    v20.push_back(StateRow(DIGIT, _TOKEN_BREAK_STATE, 1));
    _states_map[20] = v20;

    /* 30-31: period first: we are looking for .. operator */
    std::vector<StateRow> v30;
    v30.push_back(StateRow(PERIOD, 31));
    _states_map[30] = v30;
    std::vector<StateRow> v31;
    v31.push_back(StateRow(PERIOD, _TOKEN_END_STATE));
    _states_map[31] = v31;


    /* 40: colon first: we are looking at : or := tokens */
    std::vector<StateRow> v40;
    v40.push_back(StateRow(EQUALS, _TOKEN_END_STATE));
    v40.push_back(StateRow(NOT_ANY, _TOKEN_BREAK_STATE, 1));
    _states_map[40] = v40;

    /* 50: quote mark: string processing */
    std::vector<StateRow> v50;
    v50.push_back(StateRow(QUOTEM, _TOKEN_END_STATE));
    v50.push_back(StateRow(BACKSLASH, 51));
    v50.push_back(StateRow(QUOTEM, 50, 1)); /* not quote mark */
    _states_map[50] = v50;
    std::vector<StateRow> v51;
    v51.push_back(StateRow(NOT_ANY, 50, 1)); /* not not any => any */
    _states_map[51] = v51;

	/* 60: slash: comments section or just divide op */
	std::vector<StateRow> v60;
	v60.push_back(StateRow(SLASH, 61));
	v60.push_back(StateRow(ASTERISK, 62));
	v60.push_back(StateRow(NOT_ANY, _TOKEN_BREAK_STATE, 1));
	_states_map[60] = v60;
	std::vector<StateRow> v61;
	v61.push_back(StateRow(NEWLINE, _TOKEN_SKIP_STATE));
	v61.push_back(StateRow(NOT_ANY, 61, 1));
	_states_map[61] = v61;
	std::vector<StateRow> v62;
	v62.push_back(StateRow(ASTERISK, 63));
	v62.push_back(StateRow(NOT_ANY, 62, 1));
	_states_map[62] = v62;
	std::vector<StateRow> v63;
	v63.push_back(StateRow(SLASH, _TOKEN_SKIP_STATE));
	v63.push_back(StateRow(ASTERISK, 63));
	v63.push_back(StateRow(NOT_ANY, 62, 1));
	_states_map[63] = v63;

}

Scanner::~Scanner()
{
    if (_input_file.is_open())
        _input_file.close();

}

int Scanner::is_whitespace(char c)
{
    char ws_chars[] = { ' ', '\n', '\t', 0 };
    return (strchr(ws_chars, c) != NULL);
}

int Scanner::is_alpha(char c)
{
    char alpha_chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    return (strchr(alpha_chars, c) != NULL);
}

int Scanner::is_digit(char c)
{
    char digit_chars[] = "0123456789";
    return (strchr(digit_chars, c) != NULL);
}

Scanner::CHARTYPE Scanner::get_char_type(char c)
{
    if (is_alpha(c)) {
        return ALPHA;
    } else if (is_digit(c)) {
        return DIGIT;
    }
    switch(c) {
        case '.':
            return PERIOD;
        case ':':
            return COLON;
        case ';':
            return SEMICOLON;
        case ')':
            return BRACKET_RIGHT;
        case '(':
            return BRACKET_LEFT;
        case '+':
            return PLUS;
        case '-':
            return MINUS;
        case '/':
            return SLASH;
        case '\\':
            return BACKSLASH;
        case '!':
            return EXCLAMATIONM;
        case '*':
            return ASTERISK;
        case '&':
            return AND;
        case '"':
            return QUOTEM;
        case '=':
            return EQUALS;
        case '<':
            return LESS_THAN;
        case '_':
            return UNDERSCORE;
        case ' ':
            return SPACE;
        case '\t':
            return TAB;
        case '\n':
            return NEWLINE;
        default:
            return OTHER;
    }
}

int Scanner::get_next_state(char next_char, int curr_state)
{
    std::vector<StateRow> vec = _states_map[curr_state];
    CHARTYPE nc_type = get_char_type(next_char), ns_char_type = OTHER;
    int ns_not_flag = 0;
    for (int i=0; i < vec.size(); ++i) {
        ns_char_type = vec[i].next_char;
        ns_not_flag = vec[i].not_flag;
        if ((nc_type == ns_char_type && !ns_not_flag) |
            (nc_type != ns_char_type && ns_not_flag)) {
            return vec[i].next_state;
        } 
    }
    /* returning error state because no defined state for this char */
    return _TOKEN_ERROR_STATE;
}

Token Scanner::run_automaton(std::string *strbuffer)
{
    int curr_state = 0;
    char curr_c = 0, peek_c = 0;
    peek_c = _input_file.peek();
    /* get rid of whitespace */
    while(is_whitespace(peek_c) && strbuffer->size() == 0 && _input_file.good()) {
        _input_file.get(curr_c);
        peek_c = _input_file.peek();
    }
    
    if (!_input_file.good()) {
        return create_token(*strbuffer);
    }

    /* run state machine defined by states table */
    while (curr_state >= 0 && _input_file.good()) {
        peek_c = _input_file.peek();
        curr_state = get_next_state(peek_c, curr_state);
        if (curr_state >= 0 || curr_state == _TOKEN_END_STATE) {
            _input_file.get(curr_c);
            strbuffer->push_back(curr_c);
        } else if (curr_state == _TOKEN_SKIP_STATE) {
			/* skip token -> clear buffer, set state to 0, get rid of possible whitespace */
			_input_file.get(curr_c);
			peek_c = _input_file.peek();
			strbuffer->clear();
			curr_state = 0;
			while(is_whitespace(peek_c) && _input_file.good()) {
				_input_file.get(curr_c);
				peek_c = _input_file.peek();
			}
		}
    }    
    
    /* if buffer is empty, it means first char is invalid token */
    if (strbuffer->size() == 0) {
        _input_file.get(curr_c);
        strbuffer->push_back(curr_c);
        return create_error_token(*strbuffer);
    }

    return create_token(*strbuffer);
}

Token::TYPE Scanner::get_token_type(std::string str)
{
    /* check for keywords and symbolic tokens */
    const char *predef_strs[] = { "var", "for", "end", "in", "do", "read",
            "print", "int", "string", "bool", "assert", "..", ":", ":=",
            ";", ")", "(", "+", "-", "/", "!", "*", "&", "<", "=" };
    const Token::TYPE predef_types[] = { Token::KW_VAR, Token::KW_FOR,
            Token::KW_END, Token::KW_IN, Token::KW_DO, Token::KW_READ,
            Token::KW_PRINT, Token::KW_INT, Token::KW_STRING, Token::KW_BOOL,
            Token::KW_ASSERT, Token::DOUBLEDOT, Token::COLON, Token::INSERT,
            Token::SEMICOLON, Token::BRACKET_RIGHT, Token::BRACKET_LEFT,
            Token::OP_ADD, Token::OP_SUBT, Token::OP_DIVIS, Token::OP_NOT,
            Token::OP_MULT, Token::OP_AND, Token::OP_LT, Token::OP_EQ };
    for (int i=0; i<25; ++i) {
        if (str == predef_strs[i])
            return predef_types[i];
    }

    /* check for integer */
    int is_int = 1;
    for (int i=0; i < str.size(); ++i) {
        if (!is_digit(str[i])) {
            is_int = 0;
            break;
        }
    }
    if (is_int)
        return Token::INTEGER;

    /* check for string */
    if (str.size() > 1 && str[0] == '"' && 
        str[str.size()-1] == '"' && str[str.size()-2] != '\\')
        return Token::STRING;

    /* check for identifier */
    if (is_alpha(str[0])) {
        int is_ident = 1;
        for (int i=0; i < str.size(); ++i) {
            if (!(is_alpha(str[i]) || is_digit(str[i]) || str[i] == '_' )) {
                is_ident = 0;
                break;
            }
        }
        if (is_ident)
            return Token::IDENTIFIER;
    }

    /* cannot resolve type, return error type */
    return Token::ERROR; 
}

Token Scanner::create_token(std::string str)
{
    if (str.size() == 0) {
        if (_input_file.eof())
            return Token(Token::END_OF_FILE, str);
        return Token(Token::ERROR, str);
    }

    Token::TYPE type = get_token_type(str);
    /* strings: remove " from start and end */
    if (type == Token::STRING) {
        str.erase((str.size()-1), 1);
        str.erase(0, 1);
    }

    return Token(type, str);
}

Token Scanner::create_error_token(std::string str)
{
    return Token(Token::ERROR, str);
}

void Scanner::open_input_file(const char *filename)
{
    if (_input_file.is_open())
        _input_file.close();
    _input_file.open(filename);     
}

Token Scanner::next_token()
{
    if (_input_file.good()) {
        /* run automaton with string buffer */
        std::string *strbuffer;
        strbuffer = new std::string();
        Token t = run_automaton(strbuffer);
        delete strbuffer;
        return t;
    } else {
        /* create error token */
        return create_token(std::string());
    } 
}

} // namespace mpli
