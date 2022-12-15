#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "free_lang_f.h"
#include "text_funcs.h"
#include "tree_debug.h"

#include "lexical_analysis.h"
#include "syntax_analysis.h"

const char * def_reg = "var_in_def";
const char * call_reg = "var_in_funccall";
static FILE * output_file = 0;

//TODO enum
int translateLanguage(const char *input_file_name, const char *output_file_name)
{
    Text_info text1 = {};
    
    textCtorOnlyBuffer(&text1, input_file_name);

    printText(&text1);
    // printf("Ride to hell!\n");

    openLogs();

    Program_tokens program_tokens = {};
    programTokensCtor(text1.buf, &program_tokens);

    programTokensDump(&program_tokens);

    Node * main_node = getGeneral(&program_tokens);

    programTokensDtor(&program_tokens);
    
    TREE_DUMP_OPTIONAL(main_node, "initial tree"); 
    printIn(main_node);
    printf("\n");

    output_file = fopen(output_file_name, "w+");

    if (output_file == nullptr)
        return TRANSLATION_TERMINATED_SAVE_FILE_ERROR;

    fprintf(output_file,"jmp :main \n"
                        "hlt\n");

    handleProgram(main_node);

    fclose(output_file);

    closeLogs();

    // DBG_OUT;
    nodeDtor(&main_node);
    textDtor(&text1);

    return TRANSLATION_TERMINATED_SYNTAX_ERROR;
    return TRANSLATION_SUCCEEDEED;

}

int handleProgram(Node *node)
{
    if (!node)
        return 0;

    if (node->l_son)
        handleDefinition(node->l_son);

    handleProgram(node->r_son);

    return 0;
}

int handleDefinition(Node *node)
{
    if (!node || !(node->type == KEY_WORD && node->value.key_value == DEF))
        return -1;

    Node * func_name = node->l_son;

    fprintf(output_file,"\n%s:\n", func_name->value.var.name);

    // dumpNode(func_name);

    if (func_name->r_son)
    {

        Var variables[NUMBER_OF_VARS] = {};

        copyFromFunccallMemory(func_name, variables);

        handleFunc(func_name->r_son, variables);

        varArrayDtor(variables);

    }

    fprintf(output_file,"ret\n");

    return 0;
}

int handleFunc(Node *node, Var variables[])
{
    if (!node)
        return 0;

    // dumpNode(node->l_son);

    if (node->l_son)
        handleLangTree(node->l_son, variables);

    handleFunc(node->r_son, variables);

    return 0;
}

#define DEF_OP(op_name, priority, op_code, name_in_lang)                           \
    else if (node->value.op_value == op_code)                                      \
    {                                                                              \
        fprintf(output_file, "%s\n", #op_name);                                    \
        break;                                                                     \
    }  

int handleLangTree(Node *node, Var variables[])
{
    if (!node)
        return 0;

    int var_index = -1;

    if (node->type == OP && node->value.op_value == ASG)
    {
        handleASG(node, variables);
    
    } else
    {
        if (node->l_son)
            handleLangTree(node->l_son, variables);
        
        if (node->r_son)
            handleLangTree(node->r_son, variables);

        switch(node->type)
        {
            case OP:
            {
                if(0){}
                #include "operations.h"
                break;
            }
            case NUM:
            {
                fprintf(output_file, "push %d\n", (int)(node->value.dbl_value));
                break;
            }
            case VAR:        
            {
                var_index = getVarIndex(variables,node->value.var.name);
                fprintf(output_file, "push [%s+%d]\n", def_reg, var_index);
                break;
            }
            case FUNC:
            {
                handleFunccall(node, variables);

                break;
            }

        }
    
    }

    return 0;
}

int copyFromFunccallMemory(Node * func_name, Var variables[])
{
    size_t number_of_params = 0;

    if (func_name->l_son != nullptr)
    {
        fillVarArray(variables, func_name->l_son, 0);
        number_of_params = getFreeIndex(variables);
        // dumpVarArray(variables);

    }

    fillVarArray(variables, func_name->r_son, number_of_params);
    // dumpVarArray(variables);
    
    size_t number_of_vars = getFreeIndex(variables);
    // printf("%d\n", number_of_vars);

    for (size_t counter = 0; counter < number_of_vars; counter++)
    {
        if (counter < number_of_params)
        {
            fprintf(output_file, "push [%s+%d]\n", call_reg, counter);
        
        } else
        {
            fprintf(output_file, "push 0\n");
        }
    }

    for (size_t counter = number_of_vars - 1; counter != 0 ; counter--)
    {
        fprintf(output_file, "pop [%s+%d]\n", def_reg, counter);
    }

    return 0;

}

int handleFunccall(Node * func_name, Var variables[])
{
    fprintf(output_file, ";calling function %s\n", func_name->value.var.name);
    
    Node * arg = func_name->l_son;
    size_t arg_index_in_def = 0;
    size_t arg_index_in_call = 0;

    //copy variables
    while (arg != nullptr)
    {
        ASSERT(arg->type == VAR); // пока без исключений

        arg_index_in_def = getVarIndex(variables, arg->value.var.name);

        fprintf(output_file, "pop [%s+%d]\n", def_reg, arg_index_in_def);

        fprintf(output_file, "dup\n");

        fprintf(output_file, "push [%s+%d]\n", def_reg, arg_index_in_def);

        fprintf(output_file, "push [%s+%d]\n", call_reg, arg_index_in_call);       

        arg_index_in_call++;
        arg = arg->l_son;

        // dumpNode(arg);
        // DBG_OUT;

    }

    fprintf(output_file, "call :%s\n", func_name->value.var.name);


    return 0;

}

int handleASG(Node *node, Var variables[])
{
    int var_index = -1;

    if (node->r_son)
        handleLangTree(node->r_son, variables);

    var_index = getVarIndex(variables, node->l_son->value.var.name);

    return 0;
}

#undef DEF_OP