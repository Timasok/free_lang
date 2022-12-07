#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "calc_f.h"
#include "tree_debug.h"
#include "tree.h"

static const char * input;

const int MAX_VARIABLE_LEN = 15;

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

bool stringEquals(const char *s_1, const char * s_2)
{
    if (strcasecmp(s_1, s_2) == 0)
    {
        return true;

    }else {

        return false;
    }

}

Exp_node * getGeneral(const char * str)
{
    input = deleteSpaces(str);
    Exp_node * result = getExpression();

    assert(*input == '\0');

    return result;
}

Exp_node * getDegree()
{
    Exp_node * result = getPolice();
    Value value = {};

    while (*input == '^')
    {
        int op = *input;
        input++;
        
        Exp_node * tmp_result = getPolice();

        if (op == '^')
        {            
            value.op_value = POW;
            result = nodeConnect(OP, value, result, tmp_result);
        }
    }

#ifdef DEBUG    
    linkSonsToParent(result);
    dumpExpNode(result);
#endif

    return result;

}

Exp_node * getExpression( )
{
    Exp_node * result = getT(); // TODO T -> term

    Value value = {};

    while (*input == '+' || *input == '-')
    {
        int op = *input;
        input++;

        Exp_node * tmp_result = getT();

        if  (op == '+')
        {
            value.op_value = ADD;
            result = nodeConnect(OP, value, result, tmp_result);

        } else{
            
            value.op_value = SUB;
            result = nodeConnect(OP, value, result, tmp_result);
        
        }

    }

#ifdef DEBUG
    dumpExpNode(result);
#endif

    return result;
}

Exp_node * getT()
{
    Exp_node * result = getDegree();

    Value value = {};

    while (*input == '*' || *input == '/')
    {
        int op = *input;
        input++;

        Exp_node * tmp_result = getDegree();

        if  (op == '*')
        {
            value.op_value = MUL;
            result = nodeConnect(OP, value, result, tmp_result);


        } else{
            
            value.op_value = DIV;
            result = nodeConnect(OP, value, result, tmp_result);
        
        }

    }

#ifdef DEBUG
    dumpExpNode(result);
#endif

    return result;

}

Exp_node * getPolice() // TODO rename
{
    Exp_node * result = nullptr;

    if (*input == '(')
    {
        input++;
        result = getExpression();

        assert(*input == ')');

        input++;

    } else if (isalpha(*input))
    {
        result = getUnarOperation();

    }else
    {
        result = getNumber();

    }

    return result;
}

Exp_node * getUnarOperation()
{
    Exp_node * result = nullptr;

    size_t shift = 0;

    while (isalpha(input[shift]))
    {
        shift++;
    }

    if (input[shift] == '(')
    {
        char *func_name = (char *)calloc(shift+1, sizeof(char));
        strncpy(func_name, input, shift);

    #ifdef DEBUG  
        STRING_DUMP(input);
        STRING_DUMP(func_name);
    #endif

{
        Lex_sub * lex = getLexicalSubstitusions();  

        for (int idx = 0; idx < MAX_FUNC_NUMBER; idx++)
        {
            if (strcasecmp(func_name, lex[idx].initial) == 0)
            {
            
            #ifdef DEBUG                
            //operation code is the argument printfto check!
                STRING_DUMP(lex[idx].initial);
                printf("%c\n", lex[idx].parsed[3]);
            #endif

                result = createOp(lex[idx].parsed[3]);
                input += shift + 1;
                break;
            }
            
        }

        LexDtor(lex);
}

        free(func_name);

        result->r_son = getExpression();
        result->l_son = createNum(0);

        linkSonsToParent(result);

        assert(*input == ')');

        input++;

    }else {

        result = getVariable();
    }

    return result;
}

Exp_node * getVariable()
{
    char value = 0;
    const char * s_old = input;

    bool var_name_not_fully_read = true;
    
    size_t shift = 0;

    if (isalpha(*input))
    {
        shift++;
    }
    
    assert(&input[shift] != s_old);

    while (var_name_not_fully_read)
    {
        if(isalpha(input[shift]) == true)
        {
            shift++;
            continue;

        } else if(input[shift] == '_')
        {
            shift++;
            while (isdigit(input[shift]) == true)
            {
                shift++;
            }

        }else{
            
            var_name_not_fully_read = false;
        }

    }

    char *var_name = (char *)calloc(shift + 1, sizeof(char));
    strncpy(var_name, input, shift);
    input += shift;

    assert(input != s_old);

    return createVar(var_name);

}

Exp_node * getNumber()
{
    double value = 0;
    const char * s_old = input;

    if (isdigit(*input))
    {
        char * end_position = nullptr;

        value = strtod(input, &end_position);
        
        if (end_position != nullptr)
            input = end_position;
    
    }
    
    assert(input != s_old);

    return createNum(value);

}