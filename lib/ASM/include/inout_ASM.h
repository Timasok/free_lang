#include "funcs_ASM.h"

#ifndef INOUT_ASM_H
#define INOUT_ASM_H

#ifdef DEBUG_MODE
#define DBG_OUT fprintf(stderr, "Compiled nicely -line: %d file: %s func: %s\n",            \                                      __LINE__, __FILE__, __FUNCTION__)

#define PARSE_ERROR(cpuPtr, condition, error_specifier)                                        \
        cpuPtr->code_of_error  |= ((condition) & error_specifier);

#endif
#ifndef DEBUG_MODE
#define DBG_OUT
#endif

int asmCtor(Asm_info *executable, char * asm_name, int maximum_quantity);
int writeAssemblerInFile(Asm_info * executable, FILE * txt_reserve);
int asmDtor(Asm_info *executable);

#endif