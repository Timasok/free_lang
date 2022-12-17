#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "funcs_CPU.h"
#include "debug_CPU.h"
#include "stack_funcs.h"

// #define DEBUG_MODE

#ifdef DEBUG_MODE
#define DBG_OUT fprintf(stderr, "Compiled nicely -line: %d file: %s func: %s\n", \
                        __LINE__, __FILE__, __FUNCTION__)

#define PARSE_ERROR(cpuPtr, condition, error_specifier) \
    cpuPtr->code_of_error |= ((condition)&error_specifier);

#endif
#ifndef DEBUG_MODE
#endif

#include "CPU_dsl.h"

bool checkSignature(CPU_info *cpu, FILE *asm_source)
{
    assert(cpu != nullptr);
    assert(asm_source != nullptr);
    fread(&cpu->signature, sizeof(int), 1, asm_source);

    int correctSignature = 'C' + 'P' * 256;

    bool matchSignature = (correctSignature == cpu->signature);

    if (matchSignature == false)
        PRINT_ERR("NOT CORRECT SIGNATURE = %s\n", &(cpu->signature));

    return matchSignature;
}

int CPU_Ctor(CPU_info *cpu, FILE *asm_source)
{
    // DBG_OUT;

    cpu->code_of_error = 0;

    if (checkSignature(cpu, asm_source) == false)
    {
        cpu->code_of_error |= CPU_ERROR_INCORRECT_SIGNATURE;
        return EXIT_FAILURE;
    }

    // DBG_OUT;
    fread(&cpu->quantity, sizeof(int), 1, asm_source);
    // DBG_OUT;
    fread(&cpu->number_of_comands, sizeof(int), 1, asm_source);
    // DBG_OUT;

    cpu->code = (int *)calloc(cpu->quantity, sizeof(int));
    fread(cpu->code, sizeof(int), cpu->quantity, asm_source);

    stackCtor(cpu->stack, REG_CAPACITY);
    stackCtor(cpu->funcs_stack, MAX_NUM_OF_FUNCS);

    cpu->ip = 0;
    cpu->log_file = fopen("./logs/cpu_log.txt", "w+");
    fflush(cpu->log_file);

    return EXIT_SUCCESS;
}

#define DEF_CMD(name, num, arg, asmcode, cpucode) \
    case num:                                     \
        cpucode break;

int draw(CPU_info *cpu, int first_graph_index)
{
    int counter = first_graph_index;

    for (int y = 1; y < cpu->win_size.height; y++)
    {
        for (int x = 1; x < cpu->win_size.width; x++)
        {
            counter++;

            if (counter == RAM_CAPACITY - 1)
            {
                break;
            }

            if (cpu->RAM[counter] != 0)
            {
                printf("■ ");
            }
            else if (cpu->RAM[counter] == 0)
            {
                printf("□ ");
            }
        }
        printf("\n");

        if (counter == RAM_CAPACITY - 1)
        {
            break;
        }
    }

    printf("\n");

    return 0;
}

// TODO make argPtr double
int operateArgs(CPU_info *cpu, int *argPtr)
{
    elem_t push = 0;
    int reg_idx = INDEX_POISON;
    int ram_idx = INDEX_POISON;
    int sum_of_reg_and_immed_value = 0;
    bool operating_with_reg_or_ram = false;

    int num_of_comand = cpu->code[cpu->ip - 1];

    if (num_of_comand & REG_MASK)
    {
        reg_idx = cpu->code[cpu->ip++];
        argPtr = &cpu->Reg[reg_idx];

        operating_with_reg_or_ram = true;

        if (num_of_comand & IMMED_MASK)
        {
            sum_of_reg_and_immed_value = cpu->Reg[reg_idx]/ACCURACY + cpu->code[cpu->ip];
            argPtr = &sum_of_reg_and_immed_value;

#ifdef CPU_DEBUG
            printf("reg_val = %d immed_value = %d\n", cpu->Reg[reg_idx], cpu->code[cpu->ip]);
            printf("sum_of_reg_and_immed_value = %d\n", sum_of_reg_and_immed_value);
#endif

            cpu->ip++;
        }
#ifdef CPU_DEBUG
        printf("*argPtr = %d\n", *argPtr);
#endif
    }
    else if (num_of_comand & IMMED_MASK)
    {
        argPtr = &cpu->code[cpu->ip++];
    }
    else
    {
        return checkPushPopForError(cpu, CHECK_FOR_IMMED_REG);
    }

    if (num_of_comand & MEM_MASK)
    {
        // printf("*****************************************************************************\n");

        // sleep(0.5);

        operating_with_reg_or_ram = true;

        ram_idx = *argPtr;
        argPtr = &cpu->RAM[ram_idx];
#ifdef CPU_DEBUG
        printf("cpu->RAM[%d] = %d\n", ram_idx, *argPtr);
#endif
    }

    int error = checkPushPopForError(cpu, CHECK_FOR_MEM);

    if (error != 0)
        return error;

    if ((num_of_comand & MASK_REMOVER) == CMD_PUSH)
    {
        
        if (operating_with_reg_or_ram)
        {
            push = (double)(*argPtr) / ACCURACY;
        }
        else
        {
            push = *argPtr;
        }

        stackPush(&cpu->stack, push);

#ifdef DO_NOT_CLEAN_RAM
        printf("pushed %g in ip = %d\n", push, cpu->ip);
        if (ram_idx != INDEX_POISON)
            cpu->RAM[ram_idx] = 0;
#endif

#ifdef CPU_DEBUG
        printf("source_of pushed %d\n", (*argPtr));
        printf("pushed %g\n", push);
        printf("\tafter = %g\n", push);
#endif
    }
    else if ((num_of_comand & MASK_REMOVER) == CMD_POP)
    {
        elem_t result = 0;
        stackPop(&cpu->stack, &result);
        int final_result = (int)(result * ACCURACY);
        *argPtr = final_result;

#ifdef CPU_DEBUG
        printf("popped %d\n", final_result);
        printf("source_of_popped %g\n", result);
        printf("\tafter = %d\n", *argPtr);
#endif
    }

    return EXIT_SUCCESS;
}

int checkPushPopForError(CPU_info *cpu, Stage stage)
{

    // switch (stage)
    // {
    //     case CHECK_FOR_IMMED_REG:
    //     {
    //         if ()
    //         CPU_ERROR_ATTEMPT_TO_POP_INTO_IMMED
    //         CPU_ERROR_INCORRECT_POP_MASK
    //         CPU_ERROR_INCORRECT_PUSH_MASK

    //         break;
    //     }
    //     case CHECK_FOR_MEM:
    //     {
    //         CPU_ERROR_ACCESSING_TO_EMPTY_RAM
    //         CPU_ERROR_MASHING_RAM
    //         break;
    //     }
    // }
    // PRINT_ERR("WRONG MASK: -line: %d file: %s func: %s\n",  __LINE__, __FILE__, __FUNCTION__);

    //     cpu->code_of_error |= CPU_ERROR_INCORRECT_PUSH_MASK;
    //     return CPU_ERROR_INCORRECT_PUSH_MASK;

    // from pop to RAM

    // if (cpu->RAM[tmp_arg] != 0)
    // {
    //     PRINT_ERR("RAM[%d] is already filled\n", tmp_arg);
    //     cpu->code_of_error |= CPU_ERROR_ACCESSING_TO_EMPTY_RAM;
    //     return CPU_ERROR_ACCESSING_TO_EMPTY_RAM;

    // } else
    // {
    //     elem_t result = 0;
    //     stackPop(&cpu->stack, &result);
    //     cpu->RAM[tmp_arg] = result/ACCURACY;
    //     return EXIT_SUCCESS;

    // }

    // from push to RAM

    // if (cpu->RAM[tmp_arg] != 0)
    // {
    //     stackPush(&cpu->stack, cpu->RAM[tmp_arg]*ACCURACY);
    //     return EXIT_SUCCESS;

    // } else
    // {
    //     PRINT_ERR("RAM[%d] is empty\n", tmp_arg);
    //     cpu->code_of_error |= CPU_ERROR_MASHING_RAM;
    //     return CPU_ERROR_MASHING_RAM;

    // }

    return 0;
}

#undef DEF_CMD

#define DEF_CMD(name, num, arg, asmcode, cpucode)                                   \
    case num:                                                                       \
    {                                                                               \
        cpu->ip++;                                                                  \
                                                                                    \
        if (num == CMD_PUSH || num == CMD_POP)                                      \
        {                                                                           \
            operateArgs(cpu, &actual_arg);                                          \
        }                                                                           \
        else                                                                        \
        {                                                                           \
            cpucode                                                                 \
        }                                                                           \
                                                                                    \
        if (!(num == CMD_DMP || num == CMD_HLT || num == CMD_IN || num == CMD_JMP)) \
        {                                                                           \
            dump_CMD_CPU(cpu->ip - 1, cpu->code[cpu->ip - 2], cpu, 1);              \
        }                                                                           \
        else                                                                        \
        {                                                                           \
            dump_CMD_CPU(cpu->ip - 1, cpu->code[cpu->ip - 1], cpu, 0);              \
        }                                                                           \
        break;                                                                      \
    }

int process(CPU_info *cpu)
{
    while (cpu->ip < cpu->quantity)
    {
        int num_of_comand = cpu->code[cpu->ip];
        elem_t first_popped = 0, second_popped = 0;
        int actual_arg;
        int first_graph_index = 0;

#ifdef CPU_DEBUG
        printf("NUMBER OF PROCESSING COMAND %d ip - %d \n", num_of_comand, cpu->ip);
#endif
        switch (num_of_comand & MASK_REMOVER)
        {
#include "comands.h"
        }

#ifdef CPU_DEBUG
        if (cpu->ip == num_of_comand)
        {
            printf("ERROR IN %d command\n", cpu->ip);
        }
#endif
    }

    return EXIT_SUCCESS;
}

#undef DEF_CMD

int CPU_Dtor(CPU_info *cpu)
{
    stackDtor(&cpu->stack);
    stackDtor(&cpu->funcs_stack);

    fclose(cpu->log_file);
    free(cpu->code);
    return EXIT_SUCCESS;
}

#include "undefines_CPU.h"
#undef PARSE_ERROR
#undef DBG_OUT