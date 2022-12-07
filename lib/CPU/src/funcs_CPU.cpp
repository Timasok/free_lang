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
#define DBG_OUT fprintf(stderr, "Compiled nicely -line: %d file: %s func: %s\n",            \
                                                __LINE__, __FILE__, __FUNCTION__)

#define PARSE_ERROR(cpuPtr, condition, error_specifier)                                        \
        cpuPtr->code_of_error  |= ((condition) & error_specifier);

#endif
#ifndef DEBUG_MODE
#define DBG_OUT
#endif

#define PRINT_ERR(...)                                                  \
        do {                                                            \
            fprintf(stderr,"\e[0;31mERROR: " );                         \
            fprintf(stderr, __VA_ARGS__);                               \
            fprintf(stderr,"\e[0m\n" );                                 \
        } while(0)


#define FIRST_POP(cpuPtr)                                                         \
            do {                                                                  \
                    if (stackPop(&cpuPtr->stack, &first_popped) == EXIT_FAILURE)  \
                    {                                                             \
                        break;                                                    \
                    }                                                             \
                } while (0)

#define SECOND_POP(cpuPtr)                                                        \
            do {                                                                  \
                    if (stackPop(&cpuPtr->stack, &second_popped) == EXIT_FAILURE) \
                    {                                                             \
                        break;                                                    \
                    }                                                             \
                } while (0)    

#define ARITHM(operation, cpuPtr)                                                    \
            do {                                                                     \
                    stackPush(&cpuPtr->stack, second_popped operation first_popped); \
                                                                                     \
                } while (0)

#define SINGLE_POP(cpuPtr, poppedPtr)                                             \
            do {                                                                  \
                    if (stackPop(&cpuPtr->stack, poppedPtr) == EXIT_FAILURE)      \
                    {                                                             \
                        break;                                                    \
                    }                                                             \
                } while (0)

#define SINGLE_PUSH(cpuPtr, element)                                              \
            do {                                                                  \
                    stackPush(&cpuPtr->stack, element);                           \
                } while (0)

#define OUT(cpuPtr, element)                                                       \
                    do {                                                           \
                        fprintf(cpuPtr->log_file, "OUT: = %g", element);           \
                        printf("\e[0;32m\nOUT: = %g\e[0m\n", element);             \
                    } while (0)

#define ARITHM_DBG(operation)                                                       \
                fprintf(cpu->log_file, "%g %s %g\n", second_popped, #operation, first_popped)


#define JUMP(cpuPtr)                                                                                      \
        do {                                                                                              \
                if (cpuPtr->code[cpuPtr->ip] < 0 || cpuPtr->code[cpuPtr->ip] >= cpuPtr->quantity)         \
                    PRINT_ERR("LABEL POINTS OUTDOORS ip = %d", cpuPtr->ip);                               \
                cpuPtr->ip = cpuPtr->code[cpuPtr->ip];                                                    \
            } while (0)

#define COND_JUMP(cpuPtr, condition)                                                \
        do {                                                                        \
                if (second_popped condition first_popped)                           \
                {                                                                   \
                    JUMP(cpuPtr);                                                   \
                    /*PRINT_ERR("Condition succeded\n");  */                        \
                } else {                                                            \
                    /*PRINT_ERR("Condition failed\n");      */                      \
                    cpuPtr->ip++;                                                   \
                }                                                                   \
            } while (0)

#define CALL(cpuPtr)                                                                                \
        do {                                                                                        \
                if (cpuPtr->code[cpuPtr->ip] < 0 || cpuPtr->code[cpuPtr->ip] >= cpuPtr->quantity)   \
                {                                                                                   \
                    break;                                                                          \
                }                                                                                   \
                stackPush(&cpuPtr->funcs_stack, cpu->ip);                                           \
                cpuPtr->ip = cpuPtr->code[cpuPtr->ip];                                              \
                /*fprintf(stderr, "CALL to ip = %d\n", cpu->ip); */                                 \
            } while (0)              

//add assert
#define RET(cpuPtr)                                                                 \
        do {                                                                        \
                elem_t ret_position;                                                \
                stackPop(&cpuPtr->funcs_stack, &ret_position);                      \
                /*fprintf(stderr, "RET to ip = %d\n", ret_position);*/              \
                cpu->ip = ++ret_position;                                           \
                                                                                    \
            } while (0)   

#define GRAPH(cpuPtr)                                                               \
        do {                                                                        \
                first_graph_index = cpuPtr->code[cpuPtr->ip++];                     \
                                                                                    \
                if (first_graph_index < 0 || first_graph_index > RAM_CAPACITY)      \
                {                                                                   \
                    printf("GRAPH_ARG_OVERFLOW!\n");                                \
                    return -1;                                                      \
                }                                                                   \
                draw(cpuPtr, first_graph_index);                                    \
            } while (0)                                                             \

#define MAKE_CIRCLE(cpuPtr)                                                         \
        do {                                                                        \
                int radius = cpuPtr->code[cpuPtr->ip++];                            \
                int center = first_graph_index                                      \
                    + cpuPtr->win_size.width*((cpuPtr->win_size.height)/2 + 1);     \
                                                                                    \
                cpuPtr->RAM[center] = 1;                                            \
            } while (0)                                                             \

#define DUMP()                                                                     \
        do {                                                                       \
                PRINT_ERR("ENABLE TO CALCULATE FACTORIAL");                        \
                                                                                   \
            } while(0)                                                             \


bool checkSignature(CPU_info * cpu, FILE * asm_source)
{
    assert(cpu != nullptr);
    assert(asm_source != nullptr);
    fread(&cpu->signature, sizeof(int), 1, asm_source); 

    int correctSignature = 'C' + 'P'*256;

    bool matchSignature = (correctSignature == cpu->signature);

    if (matchSignature == false)
        PRINT_ERR("NOT CORRECT SIGNATURE = %s\n", &(cpu->signature));

    return matchSignature;

}

int CPU_Ctor(CPU_info * cpu, FILE * asm_source)
{
    DBG_OUT;

    cpu->code_of_error = 0;

    if (checkSignature(cpu, asm_source) == false)
    {   
        cpu->code_of_error |= CPU_ERROR_INCORRECT_SIGNATURE;
        return EXIT_FAILURE;
    } 

    DBG_OUT;
    fread(&cpu->quantity, sizeof(int), 1, asm_source);    
    DBG_OUT;    
    fread(&cpu->number_of_comands, sizeof(int), 1, asm_source);
    DBG_OUT;

#ifdef USING_INT
    cpu->code = (int *)calloc(cpu->quantity, sizeof(int));
    fread(cpu->code, sizeof(int), cpu->quantity, asm_source);
#elif defined USING_DOUBLE
    cpu->code = (double *)calloc(cpu->quantity, sizeof(double));
    fread(cpu->code, sizeof(double), cpu->quantity, asm_source);
#endif

    stackCtor(cpu->stack, REG_CAPACITY);
    stackCtor(cpu->funcs_stack, MAX_NUM_OF_FUNCS);

    cpu->ip = 0;
    cpu->log_file = fopen("./logs/cpu_log.txt", "w+");
    fflush(cpu->log_file);

    return EXIT_SUCCESS;
}

#define DEF_CMD(name, num, arg, asmcode, cpucode)                            \
    case num:                                                                \
            cpucode                                                          \
            break;                                                           \

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

            }else if (cpu->RAM[counter] == 0)
            {
                printf("□ ");
            }

        }
        printf("\n");

        if (counter == RAM_CAPACITY-1)
        {
            break;
        }
    }

    printf("\n");

    return 0;
}

int operateArgs(CPU_info *cpu, int *argPtr)
{
    int reg_idx = INDEX_POISON;
    int ram_idx = INDEX_POISON;

    int num_of_comand = cpu->code[cpu->ip - 1];

    if (num_of_comand & REG_MASK)
    {
        reg_idx = cpu->code[cpu->ip++];
        argPtr  = &cpu->Reg[reg_idx];

        if (num_of_comand & IMMED_MASK)
        {
            int sum_of_reg_and_immed_value = cpu->Reg[reg_idx] + cpu->code[cpu->ip++];
            argPtr = &sum_of_reg_and_immed_value;
        }

    } else if (num_of_comand & IMMED_MASK)
    {   
        argPtr = &cpu->code[cpu->ip++];     
        
    } else 
    {
        return checkPushPopForError(cpu, CHECK_FOR_IMMED_REG);

    }

    if(num_of_comand & MEM_MASK)
    {
        printf("*****************************************************************************\n");

        sleep(0.5);
        
        ram_idx = *argPtr;
        argPtr = &cpu->RAM[ram_idx];
    }

    int error = checkPushPopForError(cpu, CHECK_FOR_MEM);

    if (error != 0)
        return error;

    if ((num_of_comand & MASK_REMOVER) == CMD_PUSH)
    {
        stackPush(&cpu->stack, *argPtr);
        
        if (ram_idx != INDEX_POISON)
            cpu->RAM[ram_idx] = 0;

    }else if((num_of_comand & MASK_REMOVER) == CMD_POP)
    {
        elem_t result = 0;
        stackPop(&cpu->stack, &result);
        *argPtr = result;
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

#define DEF_CMD(name, num, arg, asmcode, cpucode)                                \
        case num:                                                                \
        {                                                                        \
            cpu->ip++;                                                           \
                                                                                 \
            if(num == CMD_PUSH || num == CMD_POP)                                \
            {                                                                    \
                operateArgs(cpu, &actual_arg);                                   \
                                                                                 \
            }else{                                                               \
                cpucode                                                          \
            }                                                                    \
                                                                                 \
            if (!(num == CMD_DMP || num == CMD_HLT                               \
                                 || num == CMD_IN || num == CMD_JMP))            \
            {                                                                    \
                dump_CMD_CPU(cpu->ip - 1, cpu->code[cpu->ip - 2], cpu, 1);       \
            }else                                                                \
            {                                                                    \
                dump_CMD_CPU(cpu->ip - 1, cpu->code[cpu->ip - 1], cpu, 0);       \
            }                                                                    \
            break;                                                               \
        }                                                                        \


int process(CPU_info * cpu)
{   

    while (cpu->ip < cpu->quantity)
    {
        int num_of_comand = cpu->code[cpu->ip];
        elem_t first_popped = 0, second_popped = 0;
        int actual_arg;
        int first_graph_index = 0;

        // fprintf(stderr,"NUMBER OF PROCESSING COMAND %d ip - %d \n", num_of_comand, cpu->ip);

        switch(num_of_comand & MASK_REMOVER)
        {
            #include "comands.h"

        }           

    }

    return EXIT_SUCCESS;
}

#undef DEF_CMD

int CPU_Dtor(CPU_info * cpu)
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