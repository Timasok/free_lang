#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "text_funcs.h"
#include "debug_ASM.h"

#define DEF_CMD(name, num, arg, asmcode, cpucode)                                             \
    if (strcasecmp(cmd, #name) == 0)                                                          \
    {                                                                                         \
        output->number_of_comands++;                                                          \
        output->code[output->ip] = num;                                                       \
        output->ip++;                                                                         \
                                                                                              \
        if (arg)                                                                              \
        {                                                                                     \
            number_of_args = getArgs(output, arg_string);                                     \
            printToListing(asm_listing, output, input->lines[line], number_of_args);          \
            dumpCmd(output->asm_log, line, cmd, 1, number_of_args);                           \
            number_of_args = 0;                                                               \
        }                                                                                     \
        else                                                                                  \
        {                                                                                     \
                                                                                              \
            fprintf(asm_listing, "%04x\t%02x  \t\t\t\t%s\n", output->ip - 1,                      \
                    output->code[output->ip - 1], input->lines[line]);                        \
            dumpCmd(output->asm_log, line, cmd, 0, arg);                                      \
        }                                                                                     \
    }                                                                                         \
    else

#define ASSERT_OK(asmPtr)                                      \
    do                                                         \
    {                                                          \
        if (returnAsmError(asmPtr) != 0)                       \
        {                                                      \
            dumpAsm(asmPtr, __FILE__, __FUNCTION__, __LINE__); \
        }                                                      \
    } while (0)

int compile(Text_info *input, Asm_info *output)
{

    ASSERT_OK(output);

    FILE *asm_listing = fopen("./logs/source.lst", "w+");
    fflush(asm_listing);

    int number_of_args = 0;

    fprintf(asm_listing, "adr \tcmd   arg   arg2 \tsource\n");
        int line = 0;
    do
    {
        char *cmd = NULL;
        char *arg_string = NULL;
        int shift = 0;

        for (; line < input->number_of_lines; line++)
        {
            if (*input->lines[line] == '\0')
                continue;

            sscanf(input->lines[line], "%ms%ms", &cmd, &arg_string);

            // printf("COMAND - %s ARG - %s\n", cmd, arg_string);
            int len = strlen(cmd);
            int last_char = cmd[len - 1];

            fprintf(output->asm_log, "COMAND - %s COMAND_LENGTH - %d ARG - %s\n", cmd, len, arg_string);
            
            #include "comands.h"
            
            if (last_char == ':')
            {
                fprintf(output->asm_log, "command_len == %d\n", len);
                cmd[len - 1] = '\0';

                writeLabelInFile(line, cmd, input, output, asm_listing);
            }

        }

        free(cmd);
        free(arg_string);

    } while (0);

    for (int counter = 0; counter < LABELS_SIZE; counter++)
        fprintf(output->asm_log, "\tLabel[%d] = {%d, %s}\n", counter,
                output->labels[counter].address, output->labels[counter].name);

    ASSERT_OK(output);

    fclose(asm_listing);

    return EXIT_SUCCESS;
}

#undef DEF_CMD

int getFromLabels(Asm_info *output, char *label_name, int *argument)
{
    ASSERT_OK(output);

    for (int counter = 0; output->labels[counter].address != LABEL_POISON; counter++)
    {
        if (strcasecmp(label_name + 1, output->labels[counter].name) == 0)
        {
            *argument = output->labels[counter].address;

            return EXIT_SUCCESS;
        }
    }

    ASSERT_OK(output);
    output->code_of_error |= ASM_ERROR_ATTEMP_TO_USE_UNDEFINED_LABEL;

    return ASM_ERROR_ATTEMP_TO_USE_UNDEFINED_LABEL;
}

int getFromLabels(Asm_info *output, char *label_name, double *argument)
{
    ASSERT_OK(output);

    for (int counter = 0; output->labels[counter].address != LABEL_POISON; counter++)
    {
        if (strcasecmp(label_name + 1, output->labels[counter].name) == 0)
        {
            *argument = output->labels[counter].address;

            return EXIT_SUCCESS;
        }
    }

    ASSERT_OK(output);
    output->code_of_error |= ASM_ERROR_ATTEMP_TO_USE_UNDEFINED_LABEL;

    return ASM_ERROR_ATTEMP_TO_USE_UNDEFINED_LABEL;
}

int writeLabelInFile(int number_of_line, char *cmd, Text_info *input, Asm_info *output, FILE *asm_listing)
{
    ASSERT_OK(output);

    int label_value = output->ip;

    if (output->labels[LABELS_SIZE - 1].address != LABEL_POISON)
    {
        fprintf(output->asm_log, "THERE IS NO MEMORY FOR ANOTHER TEXT LABEL\n");
        return ASM_ERROR_NO_MEMORY_FOR_TXT_LABEL;
    }

    int label_idx;

    for (label_idx = 0; output->labels[label_idx].address != LABEL_POISON; label_idx++)
    {
        if (output->labels[label_idx].address == label_value && strcasecmp(output->labels[label_idx].name, cmd) == 0)
        {
            goto labelfound;
        }
    }

    output->labels[label_idx].address = label_value;
    output->labels[label_idx].name = cmd;

labelfound:

    fprintf(asm_listing, "%04x\tlb\t  %02x\t%s\n", output->ip, label_value, input->lines[number_of_line]);
    dumpCmd(output->asm_log, number_of_line, cmd, label_idx, 1);

    ASSERT_OK(output);

    return EXIT_SUCCESS;
}

#define DEF_CMD(name, num, arg, asmcode, ...) \
    case num:                                 \
        asmcode break;

//returns number_of_args
int getArgs(Asm_info *output, char *arg_string)
{
    ASSERT_OK(output);

#ifdef USING_INT
#elif defined USING_DOUBLE
    double argument = 0;    
#endif

    bool isMemory = false;
    bool isRegister = false;
    
    int argument = 0;
    int second_argument = 0;
    
    char label_name[64] = {};
    int num_of_comand = output->code[output->ip - 1];

#ifdef USING_INT
#elif defined USING_DOUBLE
    int comand_name_transfer = 0;
#endif

    switch (num_of_comand & MASK_REMOVER)
    {

    #include "comands.h"

    default:
    {
        fprintf(output->asm_log, "GET HIGH 2");
        break;
    }
        
    }

    ASSERT_OK(output);

    if (output->code[output->ip - 1] == num_of_comand)
    {
        return 0;
    
    } else if (second_argument == 0)
    {
        return 1;

    } else
    {
        return 2;
    }
}

#undef GET_REG
#undef DEF_CMD
#undef ASSERT_OK 