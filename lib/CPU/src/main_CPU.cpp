#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <string>

#include "funcs_CPU.h"
#include "debug_CPU.h"

#ifdef DEBUG_MODE
#define DBG_OUT fprintf(stderr, "Compiled nicely -line: %d file: %s func: %s\n",            \
                                                __LINE__, __FILE__, __FUNCTION__)
#endif
#ifndef DEBUG_MODE
#define DBG_OUT
#endif

int main(int argc, const char* argv[])
{

    FILE * asm_source = nullptr;

    if (argc == 2)
    {
        asm_source = fopen(argv[1], "rb");

    } else
    {
        asm_source = fopen("b.code", "rb");
    }

    // printf("asm_source %p\n", asm_source);

    CPU_info cpu;
    CPU_Ctor(&cpu, asm_source);
    // dump_CPU(&cpu);
    DBG_OUT;

    process(&cpu);

    CPU_Dtor(&cpu);
    fcloseall();

    return EXIT_SUCCESS;
}

#undef DBG_OUT