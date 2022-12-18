#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "syntax_analysis.h"

static Program_tokens * program_tokens;

const int MAX_VARIABLE_LEN = 15;

Node * getGeneral(Program_tokens * tokens)
{
    program_tokens = tokens;

    Node * main_node = createEmpty();

    getLangTree(main_node);

    ASSERT(checkTokensForEnd(program_tokens));
    program_tokens->current = 0;

    return main_node;
}

int getLangTree(Node * predecessor)
{
    Node * left  = getAssignment();
    Node * right = nullptr;

    Token * current_token = program_tokens->tokens[program_tokens->current];

    if (current_token->type == SEPARATOR && current_token->val.sep_value == ';')
    {
        program_tokens->current++;
    }
    
#ifdef SYNTAX_DEBUG
    IF_VERIFY(!checkTokensForEnd(program_tokens) && !checkForEndOfBlock(program_tokens));
#endif

    if (!checkTokensForEnd(program_tokens) && !checkForEndOfBlock(program_tokens))
    {
        right = createEmpty();
        getLangTree(right);
    }

    linkSonsToParent(predecessor, left, right);
    
    return 0;    

}

Node * getAssignment()
{
    Node * result = nullptr;
    Token * next_token = program_tokens->tokens[program_tokens->current + 1];
    Token * current_token = program_tokens->tokens[program_tokens->current];

    if (next_token->type == OP && next_token->val.op_value == '=')
    {
        result = getVariable();

        Value value = {};
        current_token = program_tokens->tokens[program_tokens->current];

#ifdef SYNTAX_DEBUG
        TOKEN_DUMP(current_token);
        printf("tokens->current = %lu\n", program_tokens->current);
        IF_VERIFY(current_token->type == OP && current_token->val.op_value == '=');
#endif
        if (current_token->type == OP && current_token->val.op_value == '=')
        {
            program_tokens->current++;
            current_token = program_tokens->tokens[program_tokens->current];

            Node * tmp_result = nullptr;

#ifdef SYNTAX_DEBUG
        TOKEN_DUMP(current_token);
        printf("tokens->current = %lu\n", program_tokens->current);
        IF_VERIFY(current_token->type == VAR && next_token->type == SEPARATOR && next_token->val.sep_value == '(');
#endif
            next_token = program_tokens->tokens[program_tokens->current + 1];
            if (current_token->type == VAR && next_token->type == SEPARATOR && next_token->val.sep_value == '(')
            {
                tmp_result = getFunc();
                // TREE_DUMP_OPTIONAL(result, "kick it");

            } else
            {
                tmp_result = getExpression();
            }

            value.op_value = ASG;
            result = nodeConnect(OP, value, result, tmp_result);

        }
        
    } else if (current_token->type == KEY_WORD)
    {
        result = getKeyWord();

    } else if (next_token->type == SEPARATOR && next_token->val.sep_value == '(')
    {
        // DBG_OUT;
        result = getFunc();

    } else
    {
        result = getExpression();   
    }

    return result;
}

Node * getKeyWord()
{
    Value value = {};
    Node * result = nullptr;

    value.key_value = program_tokens->tokens[program_tokens->current]->val.key_value;
    program_tokens->current++;

    if (value.key_value == DEF)
    {
        Node * func = getFuncDefinition();
        result = createNode(KEY_WORD, value, func, nullptr);
        linkSonsToParent(result, func, nullptr);

    } else 
    {
        Node * condition = nullptr;
        Node * block = nullptr;

        Token * current_token = program_tokens->tokens[program_tokens->current];

#ifdef SYNTAX_DEBUG
        IF_VERIFY(current_token->type == SEPARATOR && current_token->val.sep_value == '(');
#endif
        if (current_token->type == SEPARATOR && current_token->val.sep_value == '(')
        {
            program_tokens->current++;
            current_token = program_tokens->tokens[program_tokens->current];        

#ifdef SYNTAX_DEBUG
        printf("tokens->current = %lu\n", program_tokens->current);
        TOKEN_DUMP(current_token);
#endif     
            condition = getExpression();
            // DBG_OUT;

            current_token = program_tokens->tokens[program_tokens->current];        

            ASSERT(current_token->type == SEPARATOR && current_token->val.sep_value == ')');
            
            program_tokens->current++;
        
        }

        if (value.key_value == IF)
        {
            block = getIfElseBlocks();

        } else if (value.key_value == WHILE)
        {
            block = getBlock();
        }

        // DBG_OUT;

        result = createNode(KEY_WORD, value, condition, block);
        linkSonsToParent(result, condition, block);

        // TREE_DUMP_OPTIONAL(result, "key word tree");

    }  

    return result;
}

Node * getIfElseBlocks()
{
    Node * result = createEmpty();

    Node * if_block = getBlock();
    Node * else_block = nullptr;

    Token * current_token = program_tokens->tokens[program_tokens->current];

    if (current_token->type == KEY_WORD && current_token->val.key_value == ELSE)
    {
        program_tokens->current++;
        // DBG_OUT;
        else_block = getBlock();
    }

    linkSonsToParent(result, if_block, else_block);

    return result;

}

Node * getFuncDefinition()
{
    Node * func_name = getVariable();
    func_name->type = FUNC;

    Node * argument  = nullptr;
    Node * block     = nullptr;

    Token * current_token = program_tokens->tokens[program_tokens->current];

#ifdef SYNTAX_DEBUG
    printf("tokens->current = %lu\n", program_tokens->current);
    TOKEN_DUMP(current_token);
    IF_VERIFY(current_token->type == SEPARATOR && current_token->val.sep_value == '(');
#endif

    if (current_token->type == SEPARATOR && current_token->val.sep_value == '(')
    {
        program_tokens->current++;
        current_token = program_tokens->tokens[program_tokens->current];        

#ifdef SYNTAX_DEBUG
        printf("tokens->current = %lu\n", program_tokens->current);
        TOKEN_DUMP(current_token);
        DBG_OUT;
#endif         
        argument = getArgumentsNames();

        current_token = program_tokens->tokens[program_tokens->current];        

        ASSERT(current_token->type == SEPARATOR && current_token->val.sep_value == ')');
        
        program_tokens->current++;
    
    }
    
    block = getBlock();

    linkSonsToParent(func_name, argument, block);

    return func_name;

}

Node * getFunc()
{
    Node * func_name = getVariable();
    func_name->type = FUNC;

    Node * argument  = nullptr;

    Token * current_token = program_tokens->tokens[program_tokens->current];

#ifdef SYNTAX_DEBUG
    IF_VERIFY(current_token->type == SEPARATOR && current_token->val.sep_value == '(');
#endif

    if (current_token->type == SEPARATOR && current_token->val.sep_value == '(')
    {
        program_tokens->current++;
        current_token = program_tokens->tokens[program_tokens->current];        

#ifdef SYNTAX_DEBUG
        printf("tokens->current = %lu\n", program_tokens->current);
        TOKEN_DUMP(current_token);
        DBG_OUT;
#endif         
        argument = getArguments();

        current_token = program_tokens->tokens[program_tokens->current];        

        ASSERT(current_token->type == SEPARATOR && current_token->val.sep_value == ')');
        
        program_tokens->current++;
    
    }

    linkSonsToParent(func_name, argument, nullptr);

    return func_name;

}

Node * getArgumentsNames()
{
    Token * current_token = program_tokens->tokens[program_tokens->current];

    if (current_token->type == SEPARATOR && current_token->val.sep_value == ')')
        return nullptr;

    Node * result = getVariable();
    Node * possible_son = nullptr;

    current_token = program_tokens->tokens[program_tokens->current];

    if (current_token->type == SEPARATOR && current_token->val.sep_value == COMMA)
    {
        program_tokens->current++;

        possible_son = getArgumentsNames();

    }

    linkSonsToParent(result, possible_son, nullptr);

    return result;

}

Node * getArguments()
{
    Token * current_token = program_tokens->tokens[program_tokens->current];

    if (current_token->type == SEPARATOR && current_token->val.sep_value == ')')
        return nullptr;
    
    Node * result = getExpression();
    Node * possible_son = nullptr;

    current_token = program_tokens->tokens[program_tokens->current];

    if (current_token->type == SEPARATOR && current_token->val.sep_value == COMMA)
    {
        program_tokens->current++;

        possible_son = getArguments();

    }

    linkSonsToParent(result, possible_son, nullptr);

    return result;

}

Node * getBlock()
{
    Node * block = nullptr;

    Token * current_token = program_tokens->tokens[program_tokens->current];

    if (current_token->type == SEPARATOR && current_token->val.sep_value == '{')
    {
        program_tokens->current++;

        current_token = program_tokens->tokens[program_tokens->current];

#ifdef SYNTAX_DEBUG
    TOKEN_DUMP(current_token);
    printf("tokens->current = %lu\n", program_tokens->current);
    IF_VERIFY(!(current_token->type == SEPARATOR && current_token->val.sep_value == '}'));
#endif           
        if (!checkForEndOfBlock(program_tokens))
        {
            // DBG_OUT;
            block = createEmpty();
            getLangTree(block);

        }

        current_token = program_tokens->tokens[program_tokens->current];

#ifdef SYNTAX_DEBUG
    TOKEN_DUMP(current_token);
    printf("tokens->current = %lu\n", program_tokens->current);
#endif    

        ASSERT(current_token->type == SEPARATOR && current_token->val.sep_value == '}');
        program_tokens->current++;

    }

    return block;

}

Node * getExpression()
{
    Node * result = getComparison();

    if (!checkTokensForEnd(program_tokens))
    {
        Value value = {};
        Token * current_token = program_tokens->tokens[program_tokens->current];

#ifdef SYNTAX_DEBUG
        TOKEN_DUMP(current_token);
        printf("tokens->current = %lu\n", program_tokens->current);
        WHILE_VERIFY(current_token->type == OP && (current_token->val.op_value == '+' || current_token->val.op_value == '-'));
#endif

        while (current_token->type == OP && (current_token->val.op_value == '>' || current_token->val.op_value == '<' 
        || current_token->val.op_value == EQUAlS || current_token->val.op_value == BELOW_OR_EQUALS 
        || current_token->val.op_value == ABOVE_OR_EQUALS))
        {
            INCREMENT_TOKENS;
            
            Node * tmp_result = getComparison();

#ifdef SYNTAX_DEBUG
            IF_VERIFY(current_token->val.op_value == '+');
#endif
            switch(current_token->val.op_value)
            {
                case EQUAlS:
                {
                    value.op_value = EQUAlS;
                    result = nodeConnect(OP, value, result, tmp_result);
                    break;
                }
                case NOT_EQUAlS:
                {
                    value.op_value = NOT_EQUAlS;
                    result = nodeConnect(OP, value, result, tmp_result);
                    break;
                }
                case BELOW_OR_EQUALS:
                {
                    value.op_value = BELOW_OR_EQUALS;
                    result = nodeConnect(OP, value, result, tmp_result);
                    break;
                }
                case ABOVE_OR_EQUALS:
                {
                    value.op_value = ABOVE_OR_EQUALS;
                    result = nodeConnect(OP, value, result, tmp_result);
                    break;
                }
                case BELOW:
                {
                    value.op_value = BELOW;
                    result = nodeConnect(OP, value, result, tmp_result);
                    break; 
                }
                case ABOVE:
                {
                    value.op_value = ABOVE;
                    result = nodeConnect(OP, value, result, tmp_result);
                    break;
                }
            };

            current_token = program_tokens->tokens[program_tokens->current];

#ifdef SYNTAX_DEBUG
            WHILE_VERIFY(current_token->type == OP && (current_token->val.op_value == '+' || current_token->val.op_value == '-'));
#endif
        
        
        }
    }

#ifdef SYNTAX_DEBUG
    dumpExpNode(result);
#endif

    // TREE_DUMP_OPTIONAL(result, "where all crushes");

    return result;
}

Node * getComparison()
{
    Node * result = getTerm();

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
            INCREMENT_TOKENS;
            
            Node * tmp_result = getTerm();

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

            current_token = program_tokens->tokens[program_tokens->current];

#ifdef SYNTAX_DEBUG
            WHILE_VERIFY(current_token->type == OP && (current_token->val.op_value == '+' || current_token->val.op_value == '-'));
#endif
        }
    }

#ifdef SYNTAX_DEBUG
    dumpExpNode(result);
#endif

    // TREE_DUMP_OPTIONAL(result, "where all crushes");

    return result;

}

Node * getTerm()
{
    Node * result = getDegree();

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
            INCREMENT_TOKENS;
            Node * tmp_result = getDegree();

            if  (current_token->val.op_value == '*')
            {
                value.op_value = MUL;
                result = nodeConnect(OP, value, result, tmp_result);

            } else
            {    
                value.op_value = DIV;
                result = nodeConnect(OP, value, result, tmp_result);
            
            }

            current_token = program_tokens->tokens[program_tokens->current];
            
#ifdef SYNTAX_DEBUG
            WHILE_VERIFY(current_token->type == OP && (current_token->val.op_value == '*' || current_token->val.op_value == '/'));
#endif

        }

    }

#ifdef SYNTAX_DEBUG
    dumpExpNode(result);
#endif

    return result;

}

Node * getDegree()
{
    Node * result = getPolice();
    
    // DBG_OUT;

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
#ifdef SYNTAX_DEBUG
            printf("tokens->current = %lu\n", program_tokens->current);
            TOKEN_DUMP(program_tokens->tokens[program_tokens->current]);
            DBG_OUT;
#endif
            INCREMENT_TOKENS;
            Node * tmp_result = getPolice();

            value.op_value = POW;
            result = nodeConnect(OP, value, result, tmp_result);

            current_token = program_tokens->tokens[program_tokens->current];

#ifdef SYNTAX_DEBUG
            WHILE_VERIFY(current_token->type == OP && current_token->val.op_value == '^');
#endif
        }

    }

#ifdef SYNTAX_DEBUG
    linkSonsToParent(result);
    dumpExpNode(result);
#endif

    return result;

}

Node * getPolice() // TODO rename
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

        result = getExpression();
        current_token = program_tokens->tokens[program_tokens->current];
        
#ifdef SYNTAX_DEBUG
        TOKEN_DUMP(current_token);
        printf("tokens->current = %lu\n", program_tokens->current);
#endif    

        ASSERT(current_token->type == SEPARATOR && current_token->val.sep_value == ')');

        program_tokens->current++;

    } else if (current_token->type == OP || current_token->type == KEY_WORD || current_token->type == VAR)
    {
        // DBG_OUT;
#ifdef SYNTAX_DEBUG
        IF_VERIFY(current_token->type == OP || current_token->type == KEY_WORD || current_token->type == VAR);
#endif
        result = getUnarOperation();

    }else if (current_token->type == NUM)
    {
        // DBG_OUT;
#ifdef SYNTAX_DEBUG
        IF_VERIFY(current_token->type == NUM);
#endif
        result = getNumber();

    } else
    {
        DBG_OUT;
    }
    return result;
}

Node * getUnarOperation()
{
    Node * result = nullptr;

    Token * current_token = program_tokens->tokens[program_tokens->current];

#ifdef SYNTAX_DEBUG
    TOKEN_DUMP(current_token);
    printf("tokens->current = %lu\n", program_tokens->current);
    IF_VERIFY(current_token->type == OP && getPriority(current_token) == UNAR_PRIORITY);
#endif

    if (current_token->type == OP && getPriority(current_token) == UNAR_PRIORITY)
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
                
            result->r_son = getExpression();
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

    } else 
    {
        result = getVariable();
    }

    return result;
}

Node * getVariable()
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

Node * getNumber()
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