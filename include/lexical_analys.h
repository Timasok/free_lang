#ifndef LEXICAL_ANALYS_H
#define LEXICAL_ANALYS_H

#include "tree.h"
#include "text_funcs.h"

const char * deleteSpaces(const char * str);
char * getNextLineSlice(const char * line);

bool stringEquals(const char *s_1, const char * s_2);

struct  Lex_sub
{
    char *initial;
    char parsed[5];

};

Lex_sub * getLexicalSubstitusions();
int LexDtor(Lex_sub *lex);

const int INITIAL_TOKEN_NUMBER = 64;
const char SEPARATOR = '~';

struct Token
{
    Node_type type;
    Value val;

};

//maybe add capacity
struct  Program_tokens
{
    Token ** tokens;
    size_t size;
    size_t current;

};

Token * tokenCtor(Node_type type, Value val);
int tokenDtor(Token * token);

int programTokensCtor(const char * input_line, Program_tokens *program_tokens);
int programTokensDtor(Program_tokens *program_tokens);

#endif