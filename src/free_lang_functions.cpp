#include <string.h>
#include <stdlib.h>

#include "cpu_control.h"

#include "tree.h"
#include "text_funcs.h"
#include "tree_debug.h"
#include "free_lang_functions.h"

#include "lexical_analysis.h"
#include "syntax_analysis.h"

const char * def_reg  = "var_in_def";
const char * call_reg = "var_in_funccall";
const char * ret_reg  = "ret_value";

static FILE * output_file  = 0;
static int if_else_counter = 0;
static int while_counter = 0;

#define PRINT_COMPARASION_SIGN(comparasion_operator, asm_transcription)                 \
    do {                                                                                \
        case comparasion_operator:                                                      \
        {                                                                               \
            fprintf(output_file, #asm_transcription);                                   \
            break;                                                                      \
        }                                                                               \
    } while (0)                                                                         \

//enter func or exit func
static int increaseOrDecreaseRegister(def_change_mode mode)
{
    const char * operation = nullptr;

    switch (mode)
    {
        case INCREASE:
        {
            operation = "add";
            break;
        }
        case DECREASE:
        {
            operation = "sub";
            break;
        }
    }

    fprintf(output_file, "\npush %s\n"
                         "push %d\n"
                         "%s\n"
                         "pop %s\n\n"
                        , def_reg, SHIFT_IN_DEF, operation, def_reg);

    return 0;
}

Translation_result translateLanguage(const char *input_file_name, const char *output_file_name)
{
    Text_info text1 = {};
    
    textCtorOnlyBuffer(&text1, input_file_name);

    printText(&text1);//log

    openLogs();

    Program_tokens program_tokens = {};
    programTokensCtor(text1.buf, &program_tokens);

    programTokensDump(&program_tokens);//log

    Node * main_node = getGeneral(&program_tokens);

    programTokensDtor(&program_tokens);

    if (main_node == nullptr)
    {
        dtorDuringTrandslation(&main_node, &text1);
        return TRANSLATION_TERMINATED_SYNTAX_ERROR;
    }
    
    // TREE_DUMP_OPTIONAL(main_node, "initial tree"); 

    output_file = fopen(output_file_name, "w+");

    if (output_file == nullptr)
    {
        dtorDuringTrandslation(&main_node, &text1);
        return TRANSLATION_TERMINATED_SAVE_FILE_ERROR;
    }

    translateToAST(main_node);

    fclose(output_file);

    dtorDuringTrandslation(&main_node, &text1);

    return TRANSLATION_SUCCEEDEED;

}

int dtorDuringTrandslation(Node **main_node, Text_info * text)
{
    closeLogs();
    nodeDtorComplete(main_node);
    textDtor(text);

    return 0;
}

int translateToAST(Node *node)
{
    fprintf(output_file,"push %d\n"
                        "pop %s\n", VAR_IN_FUNCCALL_INDEX, call_reg);

    fprintf(output_file,"push %d\n"
                        "pop %s\n", VAR_IN_DEF_INDEX, def_reg);

    fprintf(output_file,"call :main \n"
                        "hlt\n");

    handleProgram(node);

    return 0;
}

int handleProgram(Node *node)
{
    BASIC_PTR_ASSERT(node);    

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

    increaseOrDecreaseRegister(INCREASE);

    if (func_name->r_son)
    {

        Var variables[NUMBER_OF_VARS] = {};

        copyFromFunccallMemory(func_name, variables);

        handleFunc(func_name->r_son, variables);

        varArrayDtor(variables);

    }

    increaseOrDecreaseRegister(DECREASE);
    fprintf(output_file,"ret\n");

    return 0;
}

int handleFunc(Node *node, Var variables[])
{
    BASIC_PTR_ASSERT(node);   

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
    BASIC_PTR_ASSERT(node);   

    int var_index = -1;

    if (node->type == OP && node->value.op_value == ASG)
    {
        var_index = handleASG(node, variables);
    
    } else if (getPriority(node) == ASG_PRIORITY)
    {
        return COMPAR;

    } else if (node->type == KEY_WORD && node->value.key_value == IF)
    {
        handleIfElse(node, variables);

    } else if (node->type == KEY_WORD && node->value.key_value == WHILE)
    {
        handleWhile(node, variables);

    } else if(node->type == FUNC)
    {
        handleFunccall(node, variables);
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
                var_index = getVarIndex(variables, node->value.var.name);

                // fprintf(output_file, "push [%s+%d]\n", def_reg, var_index);
                fprintf(output_file, "push [%s+%d] ;%s\n", def_reg, var_index, node->value.var.name);
                break;
            }

        }
    
    }

    return 0;
}

#undef DEF_OP

int copyFromFunccallMemory(Node * func, Var variables[])
{
    size_t number_of_params = 0;

    if (func->l_son != nullptr)
    {
        fillVarArray(variables, func->l_son, 0);
        number_of_params = getFreeIndex(variables);
        // printf("number_of_params = %d\n", number_of_params);

        // dumpVarArray(variables);

    }

    fillVarArray(variables, func->r_son, 0);
    // dumpVarArray(variables);
    
    size_t number_of_vars = getFreeIndex(variables);
    // printf("number_of_vars = %d\n", number_of_vars);

    if (number_of_vars == 0)
        return 0;

    for (size_t counter = 0; counter < number_of_params; counter++)
    {
        fprintf(output_file, "push [%s+%d]\n", call_reg, counter);

    }

    for (int counter = number_of_params - 1; counter >= 0 ; counter--)
    {
        fprintf(output_file, "pop [%s+%d]\n", def_reg, counter);
    }

    // fprintf(output_file, "pop [%s+0]\n", def_reg);

    return 0;

}

int handleFunccall(Node * func, Var variables[])
{
    fprintf(output_file, ";calling function %s\n", func->value.var.name);
    
    if (handleIntrinsic(func, variables))
        return 0;
    
    Node * arg = func->l_son;

    size_t arg_index_in_call = 0;

    //copy variables
    while (arg != nullptr)
    {
        pushArgInFuncall(arg, variables);

        fprintf(output_file, "pop [%s+%d]\n", call_reg, arg_index_in_call);       

        arg_index_in_call++;
        arg = arg->l_son;

    }

    fprintf(output_file, "call :%s\n", func->value.var.name);

    return 0;

}

int handleASG(Node *node, Var variables[])
{
    int var_index = -1;

    if (node->r_son)
        handleLangTree(node->r_son, variables);

    if (node->r_son->type == FUNC)
        fprintf(output_file, "push %s\n", ret_reg);        

    var_index = getVarIndex(variables, node->l_son->value.var.name);

    fprintf(output_file, "pop [%s+%d]\n", def_reg, var_index);

    return var_index;
}

int handleComparision(Node * node, Var variables[])
{
    if (node->l_son)
        handleLangTree(node->l_son, variables);

    if (node->r_son)
        handleLangTree(node->r_son, variables);

    // switch(node->value.op_value)
    // {
    //     case EQUALS:
    //     {
    //         fprintf(output_file, "jne");
    //         break;
    //     }
    //     case NOT_EQUALS:
    //     {
    //         fprintf(output_file, "je");
    //         break;
    //     }
    //     case BELOW_OR_EQUALS:
    //     {
    //         fprintf(output_file, "ja");
    //         break;
    //     }
    //     case ABOVE_OR_EQUALS:
    //     {
    //         fprintf(output_file, "jb");
    //         break;
    //     }
    //     case BELOW:
    //     {
    //         fprintf(output_file, "jae");
    //         break;
    //     }
    //     case ABOVE:
    //     {
    //         fprintf(output_file, "jbe");
    //         break;
    //     }
    // };

    switch(node->value.op_value)
    {
        PRINT_COMPARASION_SIGN(EQUALS, jne);
        PRINT_COMPARASION_SIGN(NOT_EQUALS, je);
        PRINT_COMPARASION_SIGN(BELOW_OR_EQUALS, ja);
        PRINT_COMPARASION_SIGN(ABOVE_OR_EQUALS, jb);
        PRINT_COMPARASION_SIGN(BELOW, jae);
        PRINT_COMPARASION_SIGN(ABOVE, jbe);
    };

    return 0;
}

int handleIfElse(Node * if_node, Var variables[])
{
    BASIC_PTR_ASSERT(if_node->r_son);

    int current_if_else = if_else_counter;
    if_else_counter++;

    fprintf(output_file, "push 0\n");  

    if ( handleLangTree(if_node->l_son, variables) == COMPAR )
    {
        fprintf(output_file, "clean\n"); 
        handleComparision(if_node->l_son, variables);

    } else
    {
        fprintf(output_file, "je");
    }

    fprintf(output_file, " :else_%d\n", current_if_else);  

    handleLangTree(if_node->r_son->l_son, variables);

    fprintf(output_file,"jmp :endif_%d\n" 
                        "else_%d:\n", 
                        current_if_else, current_if_else);  

    handleLangTree(if_node->r_son->r_son, variables);

    fprintf(output_file, "endif_%d:\n", current_if_else);  


    return 0;

}

int handleWhile(Node * if_node, Var variables[])
{
    BASIC_PTR_ASSERT(if_node->r_son);

    int current_while = while_counter;
    while_counter++;

    fprintf(output_file, "while_%d:\n", current_while);  
    fprintf(output_file, "push 0\n");  

    if ( handleLangTree(if_node->l_son, variables) == COMPAR )
    {
        fprintf(output_file, "clean\n"); 
        handleComparision(if_node->l_son, variables);

    } else
    {
        fprintf(output_file, "je");
    }

    fprintf(output_file, " :end_while_%d\n", current_while);  

    handleLangTree(if_node->r_son, variables);

    fprintf(output_file,"jmp :while_%d\n", current_while);  

    fprintf(output_file, "end_while_%d:\n", current_while);  

    return 0;

}

#define DEF_INTRINSIC(macro_name, name_in_lang, code)                               \
    else if (stringEquals(func_name, name_in_lang))                                 \
    {                                                                               \
        result = true;                                                              \
        code                                                                        \
    }                                                                               \

bool handleIntrinsic(Node * func, Var variables[])
{
    bool result = false;
    const char * func_name = func->value.var.name;

    if (0){}
    #include "intrinsics.h"

    return result;
}

int pushArgInFuncall(Node *arg, Var variables[])
{
    size_t arg_index_in_def = 0;

    if (arg->type == VAR)
    {
        arg_index_in_def = getVarIndex(variables, arg->value.var.name);

        fprintf(output_file, "push [%s+%d]\n", def_reg, arg_index_in_def);
    
    } else if (arg->type == NUM)
    {
        fprintf(output_file, "push %d\n", (int)arg->value.dbl_value);
    
    } else
    {   
        PRINT_ERROR_CONSOLE("LANG_ERROR_INCORRECT_ARGUMENT");
        return -1;
    }

    return 0;
}

#undef DEF_INTRINSIC