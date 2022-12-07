#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>

#include "debug_CPU.h"


int dump_CPU(CPU_info *cpu)
{
    fprintf(cpu->log_file, "\nSIGNATURE: %s\n"
                             "QUANTITY:  %d\n"
                             "NUMBER OF COMANDS: %d\n"
                      ,&(cpu->signature), cpu->quantity, cpu->number_of_comands);     

    for (int ip = 0; ip < cpu->quantity; ip++)
        fprintf(cpu->log_file, "%d\t", cpu->code[ip]);        

    fprintf(cpu->log_file, "\n");
    // fclose(cpu->log_file);

    return 0;
}

int dump_CMD_CPU(int ip, int CMD, CPU_info *cpu, bool stackChanged)
{
    fprintf(cpu->log_file, "\nComand - %d", CMD);
    // printBits(CMD);
    fprintf(cpu->log_file, "\tip - %d\n", ip);

    fprintf(cpu->log_file, "\tREGISTER\n\t");
    for (int idx = 1; idx < REG_CAPACITY; idx++)
    {
        fprintf(cpu->log_file, "[%d] ", cpu->Reg[idx]);
    }
    
    fprintf(cpu->log_file, "\tRAM\n\t");
    for (int idx = 0; idx < 32; idx++)
    {
        fprintf(cpu->log_file, "[%d] ", cpu->RAM[idx]);
    }

    if (stackChanged)
    {
        fprintf(cpu->log_file, "\nSTACK CHANGED!");
        printStack(&cpu->stack, cpu->log_file);
    }
    
    return 0;
}

int printBits(int x)
{
    // do{
    //     printf("%d", x%2);
    //     x /= 2;

    // }while(x);

    return EXIT_SUCCESS;
}