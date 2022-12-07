#ifndef FUNCS_CPU_H
#define FUNCS_CPU_H

#include "stack_funcs.h"
#include "cpu_control.h"

const int MAX_NUM_OF_FUNCS = 32;
const int MAX_STACK_SIZE   = 1024;
const int REG_CAPACITY     = 5;
const int RAM_CAPACITY     = 4096;

const int INDEX_POISON = -1;
const int ACCURACY = 1000;
const int MASK_REMOVER = 0x1F;

#define DEF_CMD(name, num, arg, ...) \
            CMD_##name = num,

struct Window_size
{
    int height = 31;
    int width  = 40;
};

struct CPU_info
{

    FILE * log_file;
    Stack stack;
    Stack funcs_stack;
    Window_size win_size;

    unsigned int code_of_error;
    int signature;
    int number_of_comands;
    int ip;
    int quantity;
    int * code;
    int Reg[REG_CAPACITY] = {};
    int RAM[RAM_CAPACITY] = {};

};

enum CMD
{
    #include "comands.h"
};

enum Masks
{
    IMMED_MASK = 1 << 5,
    REG_MASK   = 1 << 6,
    MEM_MASK   = 1 << 7,

};

enum Stage
{
    CHECK_FOR_IMMED_REG,
    CHECK_FOR_MEM,

};

#undef DEF_CMD
bool checkSignature(CPU_info * cpu, FILE * asm_source);
int CPU_Ctor(CPU_info * cpu, FILE * asm_source);

int checkPushPopForError(CPU_info *cpu, Stage stage);

int draw(CPU_info *cpu, int first_graph_index);

int operateArgs(CPU_info *cpu, int *arg);
int process(CPU_info * cpu);
int CPU_Dtor(CPU_info * cpu);

#endif