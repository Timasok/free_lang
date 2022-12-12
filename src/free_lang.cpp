#include <stdio.h>
#include <stdlib.h>

#include "free_lang_f.h"

int main(int argc, const char* argv[])
{
    char input_file[MAX_FILE_NAME_LENGTH] = "./timasok_sources/input.tim";
    char output_file[MAX_FILE_NAME_LENGTH] = "./assembler_progs/timasok.asm";

    if (argc == 3)
    {
        sprintf(input_file, "./timasok_sources/%s", argv[1]);
        sprintf(output_file, "./assembler_progs/%s", argv[2]);
        
    }

    switch (translateLanguage(input_file, output_file))
    {
        // case TRANSLATION_SUCCEEDEED: 
        // {
        //     char command[MAX_COMMAND_LENGTH] = {};
        //     sprintf(command, "./Make %s", output_file);
        //     system(command);
         
        //     break;
        // }
        default:
        {

            break;
        }
    }

    return 0;
}