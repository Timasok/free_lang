#ifndef DEBUG_H
#define DEBUG_H

#include "funcs_CPU.h"
#include "general_debug.h"

enum CPU_Errors
{
    CPU_ERROR_INCORRECT_SIGNATURE        = 1 << 0,
    CPU_ERROR_INCORRECT_PUSH_MASK        = 1 << 1,
    CPU_ERROR_INCORRECT_POP_MASK         = 1 << 2,
    CPU_ERROR_ACCESSING_TO_EMPTY_RAM     = 1 << 3,
    CPU_ERROR_MASHING_RAM                = 1 << 4,
    CPU_ERROR_ATTEMPT_TO_POP_INTO_IMMED  = 1 << 5,
    
};

int printBits(int x);
int dump_CMD_CPU(int ip, int CMD, CPU_info * cpu, bool stackChanged);
int dump_CPU(CPU_info *cpu);

int openLogs();
int closeLogs();

#endif