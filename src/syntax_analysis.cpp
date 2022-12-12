#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "syntax_analysis.h"

static const char * input;

const int MAX_VARIABLE_LEN = 15;

Node * getGeneral(Program_tokens * program_tokens)
{
    // printf("tokens->size = %lu\n", program_tokens->size);
    Node * result = getAssignment(program_tokens);

    ASSERT(checkTokensForEnd(program_tokens));
    program_tokens->current = 0;

    return result;
}

// Node * getBlock(Program_tokens *program_tokens)
// {
//     Node * result = getExpression(program_tokens);

//     if (!checkTokensForEnd(program_tokens))
//     {


//     }    
//     // Node * tmp_result = getExpression(program_tokens);
    

// }

Node * getAssignment(Program_tokens *program_tokens)
{

    Node * result = nullptr;
    Token * next_token = program_tokens->tokens[program_tokens->current + 1];

    if (next_token->type == OP && next_token->val.op_value == '=')
    {
        result = getVariable(program_tokens);

        Value value = {};
        Token * current_token = program_tokens->tokens[program_tokens->current];

#ifdef SYNTAX_DEBUG
        TOKEN_DUMP(current_token);
        printf("tokens->current = %lu\n", program_tokens->current);
        IF_VERIFY(current_token->type == OP && current_token->val.op_value == '=');
#endif
        
        if (current_token->type == OP && current_token->val.op_value == '=')
        {
            program_tokens->current++;
            Node * tmp_result = getExpression(program_tokens);

            value.op_value = ASG;
            result = nodeConnect(OP, value, result, tmp_result);
        }
        
    } else
    {
        result = getExpression(program_tokens);
    }

    return result;
}

Node * getExpression(Program_tokens * program_tokens)
{
    Node * result = getTerm(program_tokens);

    if (!checkTokensForEnd(program_tokens))
    {
        Value value = {};
        Token * current_token = program_tokens->tokens[program_tokens->current];

#ifdef SYNTAX_DEBUG
        TOKEN_DUMP(current_token);
        printf("tokens->current = %lu\n", program_tokens->current);
        WHILE_VERIFY(current_token->type == OP && (current_token->val.op_value == '+' || current_token->val.op_value == '-'));
#endif

        while (current_token->type == OP && (current_token->val.op_value == '+' || current_token->val.op_value == '-'))
        {
            //to macro    
            INCREMENT_TOKENS;

            // if( checkTokensForEnd(program_tokens))
            //     break;
            
            // program_tokens->current++;
            
            Node * tmp_result = getTerm(program_tokens);

#ifdef SYNTAX_DEBUG
            IF_VERIFY(current_token->val.op_value == '+');
#endif
            if  (current_token->val.op_value == '+')
            {
                value.op_value = ADD;
                result = nodeConnect(OP, value, result, tmp_result);

            } else
            {    
                value.op_value = SUB;
                result = nodeConnect(OP, value, result, tmp_result);
            
            }

        }
    }

#ifdef SYNTAX_DEBUG
    dumpExpNode(result);
#endif

    // TREE_DUMP_OPTIONAL(result, "where all crushes");

    return result;
}

Node * getTerm(Program_tokens * program_tokens)
{
    Node * result = getDegree(program_tokens);

    if (!checkTokensForEnd(program_tokens))
    {
        Value value = {};
        Token * current_token = program_tokens->tokens[program_tokens->current];

#ifdef SYNTAX_DEBUG
        TOKEN_DUMP(current_token);
        printf("tokens->current = %lu\n", program_tokens->current);
        WHILE_VERIFY(current_token->type == OP && (current_token->val.op_value == '*' || current_token->val.op_value == '/'));
#endif

        while (current_token->type == OP && (current_token->val.op_value == '*' || current_token->val.op_value == '/'))
        {
            program_tokens->current++;
            Node * tmp_result = getDegree(program_tokens);

            if  (current_token->val.op_value == '*')
            {
                value.op_value = MUL;
                result = nodeConnect(OP, value, result, tmp_result);

            } else
            {    
                value.op_value = DIV;
                result = nodeConnect(OP, value, result, tmp_result);
            
            }

        }

    }

#ifdef SYNTAX_DEBUG
    dumpExpNode(result);
#endif

    return result;

}

Node * getDegree(Program_tokens * program_tokens)
{
    Node * result = getPolice(program_tokens);
    
    if (!checkTokensForEnd(program_tokens))
    {
        Value value = {};
        Token * current_token = program_tokens->tokens[program_tokens->current];

#ifdef SYNTAX_DEBUG
        TOKEN_DUMP(current_token);
        printf("tokens->current = %lu\n", program_tokens->current);
        WHILE_VERIFY(current_token->type == OP && current_token->val.op_value == '^');
#endif

        while (current_token->type == OP && current_token->val.op_value == '^')
        {
            program_tokens->current++;
            Node * tmp_result = getPolice(program_tokens);

            value.op_value = POW;
            result = nodeConnect(OP, value, result, tmp_result);

        }

    }

#ifdef SYNTAX_DEBUG
    linkSonsToParent(result);
    dumpExpNode(result);
#endif

    return result;

}

Node * getPolice(Program_tokens * program_tokens) // TODO rename
{
    Node * result = nullptr;

    Token * current_token = program_tokens->tokens[program_tokens->current];

#ifdef SYNTAX_DEBUG
    TOKEN_DUMP(current_token);
    printf("tokens->current = %lu\n", program_tokens->current);    
    IF_VERIFY(current_token->type == SEPARATOR && current_token->val.sep_value == '(');
#endif

    if (current_token->type == SEPARATOR && current_token->val.sep_value == '(')
    {
        program_tokens->current++;

        result = getAssignment(program_tokens);
        current_token = program_tokens->tokens[program_tokens->current];
        
#ifdef SYNTAX_DEBUG
        TOKEN_DUMP(current_token);
        printf("tokens->current = %lu\n", program_tokens->current);
#endif    

        ASSERT(current_token->type == SEPARATOR && current_token->val.sep_value == ')');

        program_tokens->current++;

    } else if (current_token->type == OP || current_token->type == KEY_WORD || current_token->type == VAR)
    {

#ifdef SYNTAX_DEBUG
        IF_VERIFY(current_token->type == OP || current_token->type == KEY_WORD || current_token->type == VAR);
#endif
        result = getUnarOperation(program_tokens);

    }else if (current_token->type == NUM)
    {

#ifdef SYNTAX_DEBUG
        IF_VERIFY(current_token->type == NUM);
#endif
        result = getNumber(program_tokens);

    }
    
    return result;
}

Node * getUnarOperation(Program_tokens * program_tokens)
{
    Node * result = nullptr;

    Token * current_token = program_tokens->tokens[program_tokens->current];

#ifdef SYNTAX_DEBUG
    TOKEN_DUMP(current_token);
    printf("tokens->current = %lu\n", program_tokens->current);
    IF_VERIFY(current_token->type == OP && current_token->val.op_value == SIN);
#endif

    if (current_token->type == OP && current_token->val.op_value == SIN)
    {
        result = createOp(current_token->val.op_value);
        program_tokens->current++;
    
        current_token = program_tokens->tokens[program_tokens->current];

#ifdef SYNTAX_DEBUG
        TOKEN_DUMP(current_token);
        printf("tokens->current = %lu\n", program_tokens->current);
        IF_VERIFY(current_token->type == SEPARATOR && current_token->val.sep_value == '(');
#endif

        if (current_token->type == SEPARATOR && current_token->val.sep_value == '(')
        {
            program_tokens->current++;
                
            result->r_son = getExpression(program_tokens);
            result->l_son = createNum(0);
            linkSonsToParent(result);

            current_token = program_tokens->tokens[program_tokens->current];

#ifdef SYNTAX_DEBUG
            TOKEN_DUMP(current_token);
            printf("tokens->current = %lu\n", program_tokens->current);
#endif

            ASSERT(current_token->type == SEPARATOR && current_token->val.sep_value == ')');            
            
            program_tokens->current++;

        }

    } else {

        result = getVariable(program_tokens);
    }

    return result;
}

Node * getVariable(Program_tokens * program_tokens)
{

    Token * current_token = program_tokens->tokens[program_tokens->current];

#ifdef SYNTAX_DEBUG
    TOKEN_DUMP(current_token);
    printf("tokens->current = %lu\n", program_tokens->current);
#endif

    Node * result = createVar(current_token->val.var.name);
    program_tokens->current++;

    return result;

}

Node * getNumber(Program_tokens * program_tokens)
{
    Token * current_token = program_tokens->tokens[program_tokens->current];

#ifdef SYNTAX_DEBUG
    TOKEN_DUMP(current_token);
    printf("tokens->current = %lu\n", program_tokens->current);
#endif

    Node * result = createNum(current_token->val.dbl_value);
    program_tokens->current++;

    return result;
}