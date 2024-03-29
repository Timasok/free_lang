#ifndef LEXICAL_ANALYS_H
#define LEXICAL_ANALYS_H

// #define SYNTAX_DEBUG
#define LEXER

#include "tree_debug.h"
#include "text_funcs.h"

const char * deleteSpaces(const char * str);
char * getNextLineSlice(const char * line);
char * getStrTok(char * line);

bool stringEquals(const char *s_1, const char * s_2);

const int INITIAL_TOKEN_NUMBER = 512;
const int MAX_SEP_OPER_LENGTH  = 32;
const int MAX_WORD_LENGTH      = 128;
// const char SEPARATOR = '~';

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
Token * tokenCtor(const char * var_name);

#define TOKEN_DUMP(token)                                                       \
            tokenDumpExtended(token, #token, __FILE__, __PRETTY_FUNCTION__, __LINE__)   \

int tokenDump(const Token * token);
int tokenDumpInFile(const Token *token, FILE * file);
int tokenDumpExtended(const Token * token, const char * name_of_var, const char * name_of_file, const char * name_of_func, int number_of_line);

int getPriority(const Token * token);
int tokenDtor(Token * token);

int programTokensCtor(const char * input_line, Program_tokens *program_tokens);
int programTokensDtor(Program_tokens *program_tokens);
int programTokensDump(Program_tokens *program_tokens);

int handleProgramPiece(char *line, Program_tokens *program_tokens);

char *      checkForVar       (char *line, size_t visible_len);
double      checkForNum       (char *line, size_t visible_len);
Key_word    checkForKeyWord   (char *line, size_t visible_len);
Separator   checkForSeparator (char *line, size_t visible_len);
Operator    checkForOperator  (char *line, size_t visible_len);

bool checkTokensForEnd(Program_tokens *program_tokens);
bool checkForEndOfBlock(Program_tokens *program_tokens);

#define INCREMENT_TOKENS                                \
            if (checkTokensForEnd(program_tokens))      \
                break;                                  \
            program_tokens->current++                   \

#endif