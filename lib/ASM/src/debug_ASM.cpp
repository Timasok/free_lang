#include "debug_ASM.h"

static FILE *text_logs;

int openAsmLogs()
{
    text_logs = fopen("./logs/text_log.txt", "w+");
    return 0;
    return EXIT_SUCCESS;
}

int closeAsmLogs()
{
    fclose(text_logs);
    return EXIT_SUCCESS;
}

int pushDmp(FILE *asm_log, const int argument, const bool isRegister, const bool isMemory)
{
    switch (isRegister)
    {
        case true:  
            if (argument == INVALID_REGISTER)
            {
                fprintf(asm_log, "INVALID_REGISTER_ARGUMENT\n");

            } else{
                fprintf(asm_log, "ARGUMENT: %d REGISTER: %d MEMORY: %d\n", argument, isRegister, isMemory);
            }

        case false:
            fprintf(asm_log, "ARGUMENT: %d REGISTER: %d MEMORY: %d\n", argument, isRegister, isMemory);

    }

    return EXIT_SUCCESS;

}

int dumpCmd(FILE *asm_log, const int number_of_line, const char * cmd, const int argument, const bool hasArg)
{
    switch(hasArg)
    {
       case true:  
                fprintf(asm_log, "line - %d comand - %s argument - %d\n", number_of_line, cmd, argument);
                break;
       case false:  
                fprintf(asm_log, "line - %d comand - %s\n", number_of_line, cmd);
                break;
    }
    return EXIT_SUCCESS;
    
}

int printToListing(FILE *asm_listing, Asm_info * output, const char * string, int number_of_args)
{
    switch(number_of_args)
    {
        case 0:
        {
            break;
        }
        case 1:
        {
            fprintf(asm_listing, "%04x\t%02x\t  %02x\t%02x \t\t%s\n", output->ip - 2, output->code[output->ip - 2],
                 output->code[output->ip - 1], 0, string);      
            break;
        }
        case 2:
        {
            fprintf(asm_listing, "%04x\t%02x\t  %02x\t%02x \t\t%s\n", output->ip - 3, output->code[output->ip - 3],
                 output->code[output->ip - 2], output->code[output->ip - 1], string);      
            break;
        }
        default:
        {
            printf("Everything is gonna be alright!\n");
            break;
        }
    }
    

    return 0;
}

int dumpAsm(Asm_info *output, const char *name_of_file, const char *name_of_func, int number_of_line)
{
   
    fprintf(output->asm_log, "%s at %s(%d)\n", name_of_func, name_of_file, number_of_line);
    strAsmError(output->asm_log, output->code_of_error);

    return EXIT_SUCCESS;
}

int strAsmError(FILE *asm_log, int code_of_error)
{
    fprintf(asm_log, "CODE OF ERROR: %d\n", code_of_error);
    
    return EXIT_SUCCESS;
}

int returnAsmError(Asm_info *output)
{
    output->code_of_error |= ((output->asm_file == NULL) ? ASM_ERROR_INVALID_FILE_POINTER : 0);
    
    output->code_of_error |= ((output->signature != "CP" && output->signature != "DED") ? ASM_ERROR_INCORRECT_SIGNATURE : 0);

    output->code_of_error |= (0) ? ASM_ERROR_INCORRECT_VERSION : 0;

    output->code_of_error |=   ((output->compile_once != true && output->compile_once != false) ? ASM_ERROR_INVALID_SECOND_PASS_BOOL : 0);
    
    output->code_of_error |=   ((output->number_of_comands < 0 || output->number_of_comands > output->ip) ? ASM_ERROR_INCORRECT_NUMBER_OF_COMANDS : 0);
    
    output->code_of_error |=   ((output->ip < 0) ? ASM_ERROR_INCORRECT_IP : 0);

    output->code_of_error |=   ((output->code == NULL) ? ASM_ERROR_CODE_IS_NULL : 0);

    // output->code_of_error |= (() & ASM_ERROR_LABEL_IS_DEFINED_TWICE);

    // output->code_of_error |= (() ? ASM_ERROR_ATTEMP_TO_USE_UNDEFINED_LABEL : 0);

    // // output->code_of_error |= (() ? ASM_ERROR_INCORRECT_JMP_ARG_FORMAT : 0);

    return output->code_of_error;
}
