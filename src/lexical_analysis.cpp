#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "lexical_analysis.h"

const char * deleteSpaces(const char * str)
{
    size_t initial_len = strlen(str);
    size_t new_len = 0;

#ifdef DEBUG
    STRING_DUMP(str);
#endif

    char * tmp_string = (char *)calloc(initial_len, sizeof(char));

    for (int counter = 0; counter < initial_len; counter++)
    {
        if (!isspace(str[counter]))
        {
            tmp_string[new_len] = str[counter];
            new_len++;
        }
    }

    if (new_len == initial_len)
    {
        free(tmp_string);
        return str;
    }

#ifdef DEBUG
    printf("new_len = %d\n", new_len);
    printf("string = %s new_len = %d\n", tmp_string, new_len);
#endif

    tmp_string[new_len] = '\0';

    const char * result = strdup(tmp_string);

    free(tmp_string);
    return result;  

}

//своя strtok
char * getNextLineSlice(const char * line)
{
    while (isspace(*line))
        line++;


    size_t number_of_separators = 4;
    char separator[] = {' ', '\n', '\t', '\0'};

    const char ** next_separator = (const char **)calloc(number_of_separators, sizeof(const char *));
    size_t * shifts = (size_t *)calloc(number_of_separators, sizeof(size_t));

    size_t min_shift = strlen(line);

    for (size_t counter = 0; counter < number_of_separators; counter++)
    {
        next_separator[counter] = strchr(line, separator[counter]);

        if (next_separator[counter] != nullptr)
        {
            shifts[counter] = next_separator[counter] - line;
            // printf("shift[%d] = %d\n", counter, shifts[counter]);

        }else
        {
            continue;
        }

        if (min_shift > shifts[counter])
        {
            min_shift = shifts[counter];
        }

    }

    // printf("%d\n", min_shift);

    char * processed_line = (char *)calloc(min_shift, sizeof(char *));
    strncpy(processed_line, line, min_shift);

    free(next_separator);
    free(shifts);

    return processed_line;

}

#define PRINT_ERROR(error_specifier)                                    \
        fprintf(lexer_log, "%s", #error_specifier);

static FILE * lexer_log = 0;

static int openLexLogs()
{
    lexer_log = fopen("./logs/lexer_log.txt", "w+");

    if (!lexer_log)
    {
        PRINT_ERROR_CONSOLE(LANG_ERROR_CAN_T_OPEN_LOGS);
        return LANG_ERROR_CAN_T_OPEN_LOGS;
    }

    return 0;
}

static int closeLexLogs()
{
    fclose(lexer_log);
    return 0;
}

double checkForNum(char *line, size_t * shift)
{
    double result = NAN;

    if (isdigit(*line))
    {
        char * end_position = nullptr;

        result = strtod(line, &end_position);
        
        if (end_position != nullptr)
        {
            *shift +=(end_position - line);

            // fprintf(lexer_log, "end_pos %c\n", *end_position);

            if (!isspace(*end_position))
                PRINT_ERROR(LANG_ERROR_VAR_STARTS_WITH_NUMBER);

        }

    }
        
    return result;

}

#define DEF_OP(op_name, priority, op_code, name_in_lang)                               \
            else if(op_name != NOT_OP && stringEquals(name_in_lang, processed_line))   \
            {                                                                          \
                result = op_name;                                                      \
            }                                                                          \
        
Operator checkForOperator(char *line, size_t * shift)
{
    Operator result = NOT_OP;

    if(!(*line))
        return result;

#ifdef LEX_DEBUG
    STRING_DUMP(line);
#endif

    char * processed_line = strtok(line, " \n\0");

    if (0)
    {}
    #include "operations.h"

    if (result != NOT_OP)
        *shift += strlen(processed_line) + 1;

#ifdef LEX_DEBUG
    STRING_DUMP(processed_line);
#endif
    return result;

}

#undef DEF_OP

#define DEF_KEY(key_name, key_code, name_in_lang)                                        \
            else if(key_name != NOT_KEY && stringEquals(name_in_lang, processed_line))   \
            {                                                                            \
                result = key_name;                                                       \
            }                                                                            \
        
Key_word checkForKeyWord(char *line, size_t * shift)
{
    Key_word result = NOT_KEY;

    if(!(*line))
        return result;

#ifdef LEX_DEBUG
    STRING_DUMP(line);
#endif

    char * processed_line = strtok(line, " \n\0");

    if (0)
    {}
    #include "key_words.h"

    if (result != NOT_KEY)
        *shift += strlen(processed_line) + 1;

#ifdef LEX_DEBUG
    STRING_DUMP(processed_line);
#endif

    return result;

}

#undef DEF_KEY

#define DEF_SEP(sep_name, sep_code, name_in_lang)                                        \
            else if(sep_name != NOT_SEP && stringEquals(name_in_lang, processed_line))   \
            {                                                                            \
                result = sep_name;                                                       \
            }                                                                            \
        
Separator checkForSeparator(char *line, size_t * shift)
{
    Separator result = NOT_SEP;

    if(!(*line))
        return result;

#ifdef LEX_DEBUG
    STRING_DUMP(line);
#endif

    char * processed_line = strtok(line, " \n\0");

    if (0)
    {}
    #include "separators.h"

    if (result != NOT_SEP)
        *shift += strlen(processed_line) + 1;

#ifdef LEX_DEBUG
    STRING_DUMP(processed_line);
#endif

    return result;

}

#undef DEF_SEP

#define DEF_OP(op_name, priority, op_code, name_in_lang)                     \
    else if(token->val.op_value == op_name)                                \
    {                                                                        \
        return priority;                                                     \
    }                                                                        \

int getPriority(const Token * token)
{
    if (0)
    {}
        #include "operations.h"

    return 0;
}

#undef DEF_OP

char * checkForVar(char *line, size_t * shift)
{

    if(!(*line))
        return nullptr;

#ifdef LEX_DEBUG
    STRING_DUMP(line);
#endif

    char * resulted_name = strtok(line, " \n");
    *shift += strlen(resulted_name) + 1;

#ifdef LEX_DEBUG
    STRING_DUMP(resulted_name);
#endif

    return resulted_name;

}

Token * tokenCtor(Node_type type, Value val)
{
    Token * result = (Token *)calloc(1, sizeof(Token));

    result->type = type;
    result->val  = val;

    return result;
}

Token * tokenCtor(const char * var_name)
{
    Token * result = (Token *)calloc(1, sizeof(Token));

    result->type = VAR;
    result->val.var.name  = strdup(var_name);

    return result;
}

int tokenDtor(Token * token)
{
    if (!token)
        return 0;

    if (token->type == VAR)
    {
        varDtor(token->val.var);
    }

    free(token);

    return 0;
}

int programTokensCtor(const char * input_line, Program_tokens *program_tokens)
{
    char * line = strdup(input_line);
    char * line_start = line;   
    size_t initial_len = strlen(line);

    openLexLogs();

    program_tokens->tokens = (Token **)calloc(INITIAL_TOKEN_NUMBER, sizeof(Token*));
    program_tokens->size   = 0;
    program_tokens->current = 0;

    size_t shift = 0;

    // DBG_OUT;
    while (line[shift] != '\0' && shift < initial_len)
    {
        Value val = {};
        while (isspace(line[shift]))
        {
            shift++;
            // printf("SHIFT\n");
        }

        val.dbl_value = checkForNum(&line[shift], &shift);
        if (!isnan(val.dbl_value))
        {
            // DBG_OUT;
            program_tokens->tokens[program_tokens->size++] = tokenCtor(NUM, val);
            continue;
        }

        val.sep_value = checkForSeparator(&line[shift], &shift);
        if (val.sep_value != NOT_SEP)
        {
            // DBG_OUT;
            program_tokens->tokens[program_tokens->size++] = tokenCtor(SEPARATOR, val);         
            continue;
        }

        val.op_value = checkForOperator(&line[shift], &shift);
        if (val.op_value != NOT_OP)
        {
            // DBG_OUT;
            program_tokens->tokens[program_tokens->size++] = tokenCtor(OP, val);         
            continue;
        }

        val.key_value = checkForKeyWord(&line[shift], &shift);
        if (val.key_value != NOT_KEY)
        {
            // DBG_OUT;
            program_tokens->tokens[program_tokens->size++] = tokenCtor(KEY_WORD, val);         
            continue;
        }

        val.var.name = checkForVar(&line[shift], &shift);
        if (val.var.name != nullptr)
        {
            // DBG_OUT;
            program_tokens->tokens[program_tokens->size++] = tokenCtor(val.var.name);      
            continue;
        }

        break;
    }

    free(line_start);

    return 0;

}

int programTokensDump(Program_tokens *program_tokens)
{
    for (int idx = 0; idx < program_tokens->size; idx++)
    {
        tokenDump(program_tokens->tokens[idx]);
    }

    return 0;
}

int tokenDump(const Token * token)
{
    if (!token)
        return -1;
    
    fprintf(lexer_log, "\n\ttoken %p\n", token);

    switch(token->type)
    {
        case OP:
            fprintf(lexer_log, "OPERATION %c\n", token->val.op_value);
            break;
        case KEY_WORD:
            fprintf(lexer_log, "KEY_WORD %c\n", token->val.key_value);
            break;
        case SEPARATOR:
            fprintf(lexer_log, "SEPARATOR %c\n", token->val.sep_value);
            break;
        case NUM:
            fprintf(lexer_log, "NUMBER %g\n", token->val.dbl_value);
            break;
        case VAR:
            fprintf(lexer_log, "VARIABLE \"%s\"\n", token->val.var.name);
            break;
        default:
            fprintf(lexer_log, "smth cringe\n");
            break;

    }
    return 0;

}

int tokenDumpConsole(const Token * token)
{
    if (!token)
        return -1;
    
    printf("\ttoken %p\n", token);

    switch(token->type)
    {
        case OP:
            printf("OPERATION %c\n", token->val.op_value);
            break;
        case KEY_WORD:
            printf("KEY_WORD %c\n", token->val.key_value);
            break;
        case SEPARATOR:
            printf("SEPARATOR %c\n", token->val.sep_value);
            break;
        case NUM:
            printf("NUMBER %g\n", token->val.dbl_value);
            break;
        case VAR:
            printf("VARIABLE \"%s\"\n", token->val.var.name);
            break;
        default:
            printf("smth cringe\n");
            break;

    }
    return 0;

}

int tokenDump(const Token * token, const char * name_of_var, const char * name_of_file, const char * name_of_func, int number_of_line)
{

    printf("\e[0;32m\n%s\e[0m at %s at %s(%d)\n",  name_of_var, name_of_func,
           name_of_file, number_of_line);
    tokenDumpConsole(token);

#ifndef SYNTAX_DEBUG
    // fprintf(lexer_log, "\n%s at %s at %s(%d)\n",  name_of_var, name_of_func,
    //        name_of_file, number_of_line);
    // tokenDump(token);
#else
    // printf("\e[0;32m\n%s\e[0m at %s at %s(%d)\n",  name_of_var, name_of_func,
    //        name_of_file, number_of_line);
    // tokenDumpConsole(token);
#endif

    return 0;

}

int programTokensDtor(Program_tokens *program_tokens)
{
    for (int idx = 0; idx < program_tokens->size; idx++)
    {
        tokenDtor(program_tokens->tokens[idx]);
    }

    free(program_tokens->tokens);
    
    closeLexLogs();

    return 0;
}

bool checkTokensForEnd(Program_tokens *program_tokens)
{
    if (program_tokens->current == program_tokens->size)
    {
        return true;    
    } else
    {
        return false;
    }

}

bool checkForEndOfBlock(Program_tokens *program_tokens)
{
    Token * current_token = program_tokens->tokens[program_tokens->current];
    
    if (current_token->type == SEPARATOR && current_token->val.sep_value == '}')
    {
        return true;
    } else
    {
        return false;
    }
}

#undef PRINT_ERR