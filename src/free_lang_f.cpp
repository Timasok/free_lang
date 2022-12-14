#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "free_lang_f.h"
#include "text_funcs.h"
#include "tree_debug.h"

#include "lexical_analysis.h"
#include "syntax_analysis.h"

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

    FILE *output_file = fopen(output_file_name, "w+");

    if (output_file == nullptr)
        return TRANSLATION_TERMINATED_SAVE_FILE_ERROR;

    fprintf(output_file,"jmp :main \n");

    handleProgram(main_node, output_file);
    // fprintf(output_file,"out\n"
    //                     "hlt\n");

    fclose(output_file);

    closeLogs();

    // DBG_OUT;
    nodeDtor(&main_node);
    textDtor(&text1);

    return TRANSLATION_TERMINATED_SYNTAX_ERROR;
    return TRANSLATION_SUCCEEDEED;

}

int handleProgram(Node *node, FILE *output_file)
{
    if (!node)
        return 0;

    if (node->l_son)
        handleDefinition(node->l_son, output_file);

    handleProgram(node->r_son, output_file);

    return 0;
}

int handleDefinition(Node *node, FILE *output_file)
{
    if (!node || !(node->type == KEY_WORD && node->value.key_value == DEF))
        return -1;

    Node * func = node->l_son;

    fprintf(output_file,"%s:\n", func->value.var.name);

    if (node->r_son) 
    {

        Var variables[NUMBER_OF_VARS] = {};
        fillVarArray(variables, func, 0);

        // dumpVarArray(variables);

        handleFunc(node->r_son, variables, output_file);

        varArrayDtor(variables);

    }

    fprintf(output_file,"ret\n");

    return 0;
}

int handleFunc(Node *node, Var variables[], FILE *output_file)
{
    if (!node)
        return 0;

    if (node->l_son)
        handleLangTree(node->l_son, variables, output_file);

    handleFunc(node->r_son, variables, output_file);

    return 0;
}

#define DEF_OP(op_name, priority, op_code, name_in_lang)                           \
    case op_name:                                                                  \
    {                                                                              \
        fprintf(output_file, "%s\n", #op_name);                                    \
        break;                                                                     \
    }  

int handleLangTree(Node *node, Var variables[], FILE *output_file)
{
    if (!node)
        return 0;

    if (node->l_son)
        handleLangTree(node->l_son, variables, output_file);
    
    if (node->r_son)
        handleLangTree(node->r_son, variables, output_file);

    switch(node->type)
    {
        case OP:
        {
            switch(node->value.op_value)
            {
                #include "operations.h"
                default:
                {
                    printf("Блэт");
                    break;
                }
            }
            break;
        }
        case NUM:
        {
            fprintf(output_file, "push %d\n", (int)(node->value.dbl_value));
            break;
        }
        case VAR:        
        {
            fprintf(output_file, "var %s\n", node->value.var.name);
            break;    
        }
        default:
            printf("Блэт");
            break;

    }

    return 0;
}

#undef DEF_OP