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

Node * getGeneral(const char * str)
{
    input = deleteSpaces(str);
    Node * result = getExpression();

    assert(*input == '\0');

    return result;
}

Node * getDegree()
{
    Node * result = getPolice();
    Value value = {};

    while (*input == '^')
    {
        int op = *input;
        input++;
        
        Node * tmp_result = getPolice();

        if (op == '^')
        {            
            value.op_value = POW;
            result = nodeConnect(ARITHM_OP, value, result, tmp_result);
        }
    }

#ifdef DEBUG    
    linkSonsToParent(result);
    dumpExpNode(result);
#endif

    return result;

}

Node * getExpression( )
{
    Node * result = getT(); // TODO T -> term

    Value value = {};

    while (*input == '+' || *input == '-')
    {
        int op = *input;
        input++;

        Node * tmp_result = getT();

        if  (op == '+')
        {
            value.op_value = ADD;
            result = nodeConnect(ARITHM_OP, value, result, tmp_result);

        } else{
            
            value.op_value = SUB;
            result = nodeConnect(ARITHM_OP, value, result, tmp_result);
        
        }

    }

#ifdef DEBUG
    dumpExpNode(result);
#endif

    return result;
}

Node * getT()
{
    Node * result = getDegree();

    Value value = {};

    while (*input == '*' || *input == '/')
    {
        int op = *input;
        input++;

        Node * tmp_result = getDegree();

        if  (op == '*')
        {
            value.op_value = MUL;
            result = nodeConnect(ARITHM_OP, value, result, tmp_result);


        } else{
            
            value.op_value = DIV;
            result = nodeConnect(ARITHM_OP, value, result, tmp_result);
        
        }

    }

#ifdef DEBUG
    dumpExpNode(result);
#endif

    return result;

}

Node * getPolice() // TODO rename
{
    Node * result = nullptr;

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

Node * getUnarOperation()
{
    Node * result = nullptr;

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

Node * getVariable()
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

Node * getNumber()
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