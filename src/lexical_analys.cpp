#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

char * getNextLineSlice(const char * line)
{
    while (isspace(*(line++)))
        ;

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


#define DEF_OP(op_name, priority, op_code, name_in_lang)                          \


Operator checkForOperator(const char *line, size_t * shift)
{

    char *processed_line = getNextLineSlice(line);
    *shift = strlen(processed_line);

    //func to operate the line
    // STRING_DUMP(processed_line);

    free(processed_line);

    return NOT_OP;

}

Token * tokenCtor(Node_type type, Value val)
{
    Token * result = (Token *)calloc(1, sizeof(Token));

    result->type = type;
    result->val  = val;

    return result;
}

int tokenDtor(Token * token)
{
    free(token);

    return 0;
}

int programTokensCtor(const char * input_line, Program_tokens *program_tokens)
{
    const char * line = input_line;

    program_tokens->tokens = (Token **)calloc(INITIAL_TOKEN_NUMBER, sizeof(Token));
    program_tokens->size   = 0;
    program_tokens->current = 0;

    Value val = {};
    size_t shift = 0;

    while (*line != '\0')
    {
        if (isdigit(*line))
        {
            char * end_position = nullptr;

            val.dbl_value = strtod(line, &end_position);
            
            if (end_position != nullptr)
            {
                line = end_position;
                program_tokens->tokens[program_tokens->current++] = tokenCtor(NUM, val);
            }

        }

        //TODO;
        // checkForOperator(line, &shift);
        // break;
        line += shift;
        // else if()
        // { }

    }

    return 0;

}

// programTokensRealloc

int programTokensDtor(Program_tokens *program_tokens)
{
    free(program_tokens);

}