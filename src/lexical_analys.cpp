#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "lexical_analys.h"

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

//get rid of this!!!
#define DEF_OP(op_name, priority, op_code, name_in_lang)                          \
            if(priority == 1)                                       \
            {                                                       \
                original[counter].initial = strdup(#op_name);       \
                sprintf(original[counter].parsed, "(0)%c", op_code);\
                counter++;                                          \
                if (counter = MAX_FUNC_NUMBER)                      \
                    break;                                          \
            }                                                       \

Lex_sub * getLexicalSubstitusions()
{
    Lex_sub * original = (Lex_sub *)calloc(MAX_FUNC_NUMBER, sizeof(Lex_sub));

    size_t counter = 0;

    while (1)
    {
        #include "operations.h"
        break;
    }

#ifdef DEBUG
    for (int idx = 0; idx < counter; idx++)
    {
        printf("LEX[%d] = {%s, %s}\n", idx, original[idx].initial, original[idx].parsed);

    }
#endif

    return original;

}

int LexDtor(Lex_sub *lex)
{
    if(!lex)
        return -1;

    for (int idx = 0; idx < MAX_FUNC_NUMBER; idx++)
    {
        free(lex[idx].initial);
    }

    free(lex);

    return 0;
}

#undef DEF_OP

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
        
Arithm_operator checkForArithmOperator(char *line, size_t * shift)
{
    Arithm_operator result = NOT_OP;

    if(!(*line))
        return result;

    STRING_DUMP(line);

    char * processed_line = strtok(line, " \n\0");

    if (0)
    {}
    #include "operations.h"

    if (result != NOT_OP)
        *shift += strlen(processed_line) + 1;

    //func to operate the line
    STRING_DUMP(processed_line);

    return result;

}

#undef DEF_OP

//TODO
Log_operator checkForLogOperator(char *line, size_t * shift)
{
    char *processed_line = getNextLineSlice(line);
    *shift = strlen(processed_line);

    Log_operator result = NOT_LOG_OP;

    //func to operate the line
    STRING_DUMP(processed_line);

    free(processed_line);

    return result;

}

char * checkForVar(char *line, size_t * shift)
{

    if(!(*line))
        return nullptr;

    STRING_DUMP(line);

    char * resulted_name = strtok(line, " \n");
    *shift += strlen(resulted_name) + 1;

    STRING_DUMP(resulted_name);

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
            printf("SHIFT\n");
        }

        val.dbl_value = checkForNum(&line[shift], &shift);
        if (!isnan(val.dbl_value))
        {
            DBG_OUT;
            program_tokens->tokens[program_tokens->size++] = tokenCtor(NUM, val);
            continue;
        }

        val.op_value = checkForArithmOperator(&line[shift], &shift);
        if (val.op_value != NOT_OP)
        {
            DBG_OUT;
            program_tokens->tokens[program_tokens->size++] = tokenCtor(ARITHM_OP, val);         
            continue;
        }

        // val.log_op = checkForLogOperator(line, &shift);
        // if (val.log_op != NOT_LOG_OP)
        // {
        //     program_tokens->tokens[program_tokens->size++] = tokenCtor(LOG_OP, val);         
        //     line += shift;
        //     continue;
        // }
        // while (isspace(*line))
        //     line++;

        val.var.name = checkForVar(&line[shift], &shift);
        if (val.var.name != nullptr)
        {
            DBG_OUT;
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
        case ARITHM_OP:
            fprintf(lexer_log, "ARTHM OPERATION %c\n", token->val.op_value);
            break;
        case LOG_OP:
            fprintf(lexer_log, "LOGICAL OPERATION %c\n", token->val.log_op);
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

int programTokensDtor(Program_tokens *program_tokens)
{
    for (int idx = 0; idx < program_tokens->size; idx++)
    {
        tokenDtor(program_tokens->tokens[idx]);
    }

    closeLexLogs();

    return 0;
}

#undef PRINT_ERR