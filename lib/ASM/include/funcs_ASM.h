#ifndef FUNCS_ASM_H
#define FUNCS_ASM_H

#include <stdio.h>
#include <sys/types.h>

#include "text_funcs.h"
#include "funcs_ASM.h"
#include "cpu_control.h"

const int LABELS_SIZE  = 40;//actual size only of numbered labels
const int LABEL_POISON = 0xFFFFFFFF;
const int MASK_REMOVER = 0x1F;

// const char * LABEL_NAME_POISON = "imp";

//TODO check memset in Ctor it might crash
struct Label
{
    int address;
    char * name;
};

struct Asm_info
{
    Label labels[LABELS_SIZE];
    char signature[4] = "CP";
    int version;
    int number_of_comands;
    int ip;

#ifdef USING_INT
    int * code;
#elif defined USING_DOUBLE
    double *code;
#endif
    
    bool compile_once = true;
    FILE * asm_file;
    FILE * asm_log = fopen("./logs/asm_log.txt", "w");
    int code_of_error;

};

#define DEF_CMD(name, num, arg, asmcode, ...) \
            CMD_##name = num,

enum CMD
{
    #include "comands.h"
};

#undef DEF_CMD

enum REGISTERS
{
    RAX = 1,
    RBX = 2,
    RCX = 3,
    RDX = 4,
    INVALID_REGISTER = -1,

};

#define GET_REG                                                                 \
    do {                                                                        \
        if (strcasecmp(reg_var, "rax") == 0)                                    \
        {                                                                       \
            argument = RAX;                                                     \
                                                                                \
        }else if(strcasecmp(reg_var, "rbx") == 0)                               \
        {                                                                       \
            argument = RBX;                                                     \
                                                                                \
        }else if(strcasecmp(reg_var, "rcx") == 0)                               \
        {                                                                       \
            argument = RCX;                                                     \
                                                                                \
        }else if(strcasecmp(reg_var, "rdx") == 0)                               \
        {                                                                       \
            argument = RDX;                                                     \
                                                                                \
        }                                                                       \
        else                                                                    \
        {                                                                       \
            argument = INVALID_REGISTER;                                        \
                                                                                \
        }                                                                       \
    } while(0)

enum MASKS
{
    IMMED_MASK = 1 << 5,
    REG_MASK   = 1 << 6,
    MEM_MASK   = 1 << 7,

};

int compile(Text_info *input, Asm_info *output);
int getArgs(Asm_info *output, char * arg_string);
int getFromLabels(Asm_info *output, int label_idx, int * argument);
int getFromLabels(Asm_info *output, char *label_name, double *argument);
int writeLabelInFile(int number_of_line, char *cmd, Text_info *input, Asm_info *output, FILE *asm_listing);

#endif