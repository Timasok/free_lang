#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "funcs_ASM.h"
#include "inout_ASM.h"
#include "debug_ASM.h"

FILE *text_logs;

int main(int argc, char ** argv)
{
    
    openAsmLogs();

    char * text_name = argv[1];
    char * asm_name  = argv[2];
    
    FILE *txt_reserve = fopen("./logs/auf.txt", "w+");

    Text_info source = {};
    textCtor(&source, text_name); 
    printText(&source);

    Asm_info executable = {};
    DBG_OUT;
    asmCtor(&executable, asm_name, 2 * (source.number_of_lines + 1));

    // assert(executable.asm_log != NULL);
    compile(&source, &executable);
    if (executable.compile_once == 0)
    {
        executable.ip = 0;
        compile(&source, &executable);        
    }

    for (int counter = 0; counter < executable.ip; counter++)
        fprintf(executable.asm_log, "%d\t", executable.code[counter]);

    writeAssemblerInFile(&executable, txt_reserve);

    // printf("number of comands = %d\n", executable.number_of_comands);
    // printf("ip = %d\n", executable.ip);

    asmDtor(&executable);
    textDtor(&source);
    
    closeAsmLogs();
    
    return EXIT_SUCCESS;
}
