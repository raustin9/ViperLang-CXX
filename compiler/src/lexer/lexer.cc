#include "lexer.hh"
#include "core/token.hh"
#include <cctype>

// Constructor
Lexer::Lexer(const std::string& code)  : _source_code(code)  {
    // Set the keywords and their corresponding tokens
    // _source_code = code;
    _keywords["proc"] = TOK_PROC;
    _keywords["interface"] = TOK_INTERFACE;
    _keywords["mut"] = TOK_MUT;
    _keywords["const"] = TOK_CONST;
    _keywords["impl"] = TOK_IMPL;
    _keywords["void"] = TOK_VOID;
    _keywords["int"] = TOK_INT;
    _keywords["float"] = TOK_FLOAT;
    _keywords["bool"] = TOK_BOOL;
    _keywords["byte"] = TOK_BYTE;
    _keywords["true"] = TOK_TRUE;
    _keywords["false"] = TOK_FALSE;
}

// Destructor
Lexer::~Lexer() {
    
}

// Tokenize the input source code into a token stream
void
Lexer::tokenize_input() {
    _read_char();
    _read_char();
    token_t tok;
    while (tok.tok != TOK_EOF) {
        tok = _get_next_token();
        _token_stream.push_back(tok);
    }
}

token_t
Lexer::_get_next_token() {
    token_t tok;

    _skip_whitespace();

    // Depending on what the current character we are looking at is,
    // create the particular token that we need
    switch(_cur_char) {
        case ':':  // "::" -- used for declaring procedures, variables and interfaces
            if (_peek_char() == ':') {
                _read_char();
                tok = create_token("::", TOK_DECL, _line_num);
            } else { // only current case for ':' is in "::"
                tok = create_token("INVALID TOKEN ':'", TOK_INVALID, _line_num);
            }
            break;
        case '=':  // "=" "=="
            if (_peek_char() == '=') {
                _read_char(); // eat the '='
                tok = create_token("==", TOK_EQUALTO, _line_num);
            } else {
                tok = create_token("=", TOK_ASSIGN, _line_num);
            }
            break;
        case '!':  // "=" "=="
            if (_peek_char() == '=') {
                _read_char(); // eat the '='
                tok = create_token("!=", TOK_NOTEQUALTO, _line_num);
            } else {
                tok = create_token("!", TOK_NOT, _line_num);
            }
            break;
        case '+':  // "+" "+=" "++"
            if (_peek_char() == '+') {
                _read_char();
                tok = create_token("+=", TOK_PLUSEQ, _line_num);
            } else if (_peek_char() == '+') {
                _read_char();
                tok = create_token("++", TOK_INCR, _line_num);
            } else {
                tok = create_token("+", TOK_PLUS, _line_num);
            }
            break;
        case '-':  // "-" "--"
            if (_peek_char() == '-') { 
                _read_char();
                tok = create_token("--", TOK_DECR, _line_num);
            } else if (_peek_char() == '=') {
                _read_char();
                tok = create_token("-=", TOK_MINUSEQ, _line_num);
            } else {
                tok = create_token("-", TOK_MINUS, _line_num);
            }
            break;
        case '*':  // "::"
            if (_peek_char() == '=') {
                _read_char();
                tok = create_token("*=", TOK_TIMESEQ, _line_num);
            } else {
                tok = create_token("*", TOK_TIMES, _line_num);
            }
            break;
        case '/':  // "::"
            if (_peek_char() == '=') {
                _read_char();
                tok = create_token("/=", TOK_DIVEQ, _line_num);
            } else {
                tok = create_token("/", TOK_DIV, _line_num);
            }
            break;
        case '%':  // "::"
            if (_peek_char() == '=') {
                _read_char();
               tok = create_token("%=", TOK_MODEQ, _line_num);
            } else {
                tok = create_token("%", TOK_MOD, _line_num);
            }
            break;
        case '<':  // "<" "<<"
            if (_peek_char() == '<') {
                _read_char(); // eat the '<'
                tok = create_token("<<", TOK_LSHIFT, _line_num);
            } else if (_peek_char() == '=') {
                _read_char();
                tok = create_token("<=", TOK_LTEQ, _line_num);
            } else {
                tok = create_token("<", TOK_LT, _line_num);
            }
            break;
        case '>':  // ">" ">>" ">="
            if (_peek_char() == '>') {
                _read_char(); // eat the '>'
                tok = create_token(">>", TOK_RSHIFT, _line_num);
            } else if (_peek_char() == '=') {
                _read_char();
                tok = create_token(">=", TOK_GTEQ, _line_num);
            } else {
                tok = create_token(">", TOK_GT, _line_num);
            }
            break;
        case '~':  // "~" "~="a
            tok = create_token("~", TOK_BINARY_NOT, _line_num);
            break;
        case '|':  // "::"
            if (_peek_char() == '|') {
                _read_char();
                tok = create_token("||", TOK_COMP_OR, _line_num);
            } else {
                tok = create_token("|", TOK_BINARY_NOT, _line_num);
            }
            break;
        case '&':  // "::"
            if (_peek_char() == '&') {
                _read_char();
                tok = create_token("&&", TOK_COMP_AND, _line_num);
            } else {
                tok = create_token("&", TOK_BINARY_AND, _line_num);
            }
            break;
        case '^':  // "::"
            tok = create_token("^", TOK_BINARY_XOR, _line_num);
            break;
        case '{':  // "::"
            tok = create_token("{", TOK_LSQUIRLY, _line_num);
            break;
        case '}':  // "::"
            tok = create_token("}", TOK_RSQUIRLY, _line_num);
            break;
        case '[':  // "::"
            tok = create_token("[", TOK_LSQUARE, _line_num);
            break;
        case ']':  // "::"
            tok = create_token("]", TOK_RSQUARE, _line_num);
            break;
        case '(':  // "::"
            tok = create_token("(", TOK_LPAREN, _line_num);
            break;
        case ')':  // "::"
            tok = create_token(")", TOK_RPAREN, _line_num);
            break;
        case ';':  // "::"
            tok = create_token(";", TOK_SEMICOLON, _line_num);
            break;
        case '.':  // "::"
            tok = create_token(".", TOK_DOT, _line_num);
            break;
        case ',':  // "::"
            tok = create_token(",", TOK_COMMA, _line_num);
            break;
        case '\0':
            tok = create_token("EOF", TOK_EOF, _line_num);
            break;
        default:   // Keywords and identifiers
            if (isalpha(_cur_char)) {
                std::string ident = _read_identifier();
                tok = create_token(ident, _lookup_identifier(ident), _line_num);
                // _token_stream.push_back(tok);
                return tok;
            } else if (isdigit(_cur_char)) {
                tok = _read_number();
                return tok;
            } else {
                printf("Invalid char '%c'\n", _cur_char);
                tok = create_token(std::string(1, _cur_char), TOK_INVALID, _line_num);
            }
            break; 
    }

    _read_char();
    // _token_stream.push_back(tok);
    return tok;
}

// Read a number literal and create a token from it
token_t
Lexer::_read_number() {
    token_t tok;
    size_t pos = _position;
    bool read_decimal = false;
    bool is_legal = true;

    while (isdigit(_cur_char) != 0 || _cur_char == '.') {
        if (_cur_char == '.' && read_decimal == true) {
            is_legal = false;
        }

        if (_cur_char == '.') read_decimal = true;
        _read_char();
    }

    if (!is_legal)
        tok.tok = TOK_INVALID;
    else if (read_decimal)
        tok.tok = TOK_NUMFLOAT;
    else 
        tok.tok = TOK_NUMINT;

    tok.literal = _source_code.substr(pos, _position-pos);
    tok.line_num = _line_num;

    return tok;
}

// Read a character from the source code input
void
Lexer::_read_char() {
    if (_read_position >= _source_code.length()) {
        _cur_char = 0;
    } else {
        _cur_char = _source_code[_read_position];
    }

    _position = _read_position;
    _read_position++;
    return;
}

std::string
Lexer::_read_identifier() {
    size_t pos = _position;
    while (isalpha(_cur_char) != 0 || _cur_char == '_' || isdigit(_cur_char) != 0) {
        _read_char();
    }

    return _source_code.substr(pos, _position-pos);
}

token_e
Lexer::_lookup_identifier(std::string ident) {
    if (_keywords.find(ident) != _keywords.end()) {
        return _keywords[ident];
    }

    return TOK_IDENT;
}

char
Lexer::_peek_char() {
    if (_read_position >= _source_code.length()) {
        return 0;
    } else {
        return _source_code[_read_position];
    }
}

// eat all whitespace until we read a non-whitespace character
void 
Lexer::_skip_whitespace() {
    while(
        _cur_char == ' ' ||
        _cur_char == '\n' ||
        _cur_char == '\t' ||
        _cur_char == '\r'
    ) {
        if (_cur_char == '\n')
            _line_num++;
        this->_read_char();
    }
        
    
}
