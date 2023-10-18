#include "token.hh"

std::string token_strings[] = {
    "TOK_PROC",       
    "TOK_INTERFACE", 
    "TOK_MUT",      
    "TOK_CONST",   
    "TOK_STRUCT",
    "TOK_SELF",
    "TOK_IDENT",  
    "TOK_IMPL",  
    "TOK_DECL", 

    "TOK_VOID",   
    "TOK_INT",    
    "TOK_FLOAT",  
    "TOK_BOOL",   
    "TOK_BYTE",   
                
    "TOK_NUMINT",
    "TOK_NUMFLOAT", 
                
    "TOK_TRUE",   
    "TOK_FALSE",  

    "TOK_ASSIGN",  
    "TOK_PLUS",       
    "TOK_MINUS",      
    "TOK_TIMES",      
    "TOK_DIV",        
    "TOK_MOD",        
    "TOK_LSHIFT",     
    "TOK_RSHIFT",     
    "TOK_PLUSEQ",     
    "TOK_MINUSEQ",    
    "TOK_TIMESEQ",    
    "TOK_DIVEQ",      
    "TOK_MODEQ",      
    "TOK_LSHIFTEQ",   
    "TOK_RSHIFTEQ",   

    "TOK_DOT",        
    "TOK_COMMA",      
    "TOK_QUOTE",      

    "TOK_BINARY_NOT",
    "TOK_BINARY_OR",  
    "TOK_BINARY_AND", 
    "TOK_BINARY_XOR", 

    "TOK_INCR",   
    "TOK_DECR",   

    "TOK_EQUALTO",    
    "TOK_NOTEQUALTO", 
    "TOK_LT",         
    "TOK_GT",         
    "TOK_LTEQ",       
    "TOK_GTEQ",       

    "TOK_NOT",        
    "TOK_COMP_AND",   
    "TOK_COMP_OR",    

    "TOK_RSQUIRLY",   
    "TOK_LSQUIRLY",   
    "TOK_RPAREN",     
    "TOK_LPAREN",     
    "TOK_RSQUARE",    
    "TOK_LSQUARE",    
    "TOK_SEMICOLON",  
 
    "TOK_INVALID",    
    "TOK_EOF"
};
token_t 
create_token(std::string literal, token_e token, size_t line_num) {
    token_t tok = {token, literal, line_num};
    return tok;
}

void 
print_token(token_t tok) {
    printf("Tok: '%s' Literal: '%s' on line %lu\n", token_strings[tok.tok].c_str(), tok.literal.c_str(), tok.line_num);
}
