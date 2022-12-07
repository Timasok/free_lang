#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv)
{
    char default_source_name[] = "factorial.asm";
    char asm_comand[128] = " ";

    if (argc == 2)
    {
        if (strstr(argv[1], "./assembler_progs/") == nullptr)
        {
            sprintf(asm_comand, "./assembler ./assembler_progs/%s b.code", argv[1]);
        }else
        {
            sprintf(asm_comand, "./assembler %s b.code", argv[1]);
        }
    
    }else{

        sprintf(asm_comand, "./assembler ./assembler_progs/%s b.code", default_source_name);
    }

    system(asm_comand);
    system("./cpu b.code");
    return EXIT_SUCCESS;
}